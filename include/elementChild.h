//
// Created by xskj on 2025/10/19.
//

#ifndef MYPROJECT_ELEMENTCHILD_H
#define MYPROJECT_ELEMENTCHILD_H

#include <map>
#include <string>
#include <iomanip>
#include <vector>
#include <memory>
#include "ASTLeaf.h"
#include "Element.h"
#include "Factory.h"
#include "ParseException.h"


class Lexer;          // 你已经有
class ASTree;         // 你已经有
class Parser;         // 补上这一句，关键！

namespace elementChild {

    class TreeElement;
    class OrTreeElement;
    class RepeateElement;
    class Leaf;
    class Skip;
    class Operators;
    class Expr;
    /*************************派生类：具体语法元素实现*********************************/

    //TreeElement: 解析一个 ASTree 节点,   接收一个 Parser 对象
    //用途：当某一部分语法本身又由一个子 Parser 定义（例如一个语句块或子表达式），就用 Tree 来嵌套。
    class TreeElement : public Element {
    public:
        // 解析器对象
        std::shared_ptr<Parser> parser;

        // 构造函数   引用，取别名
        explicit TreeElement(const std::shared_ptr<Parser>& p);

        // 重载match函数
        bool match(Lexer& lexer) const override;

        // 重载parse解析函数
        void parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res)  override;

    };

    // 用途：表示语法中 “A | B | C” 这样的选择。insert 支持在已有选择前插入优先级更高的分支（常用于语法构造的动态修改）。
    class OrTreeElement : public Element {
    public:
        OrTreeElement(std::vector<std::shared_ptr<Parser>>& p);
        ~OrTreeElement() = default;

        // 多个备选分支  指针数组
        vector<std::shared_ptr<Parser>> parsers;

        // 重载match函数
        bool match(Lexer& lexer) const override;

        // 选择匹配的parser进行解析
        std::shared_ptr<Parser>  choose(Lexer& lexer) const;

        // 重载parse解析函数
        // 调用 choose 获取具体分支 p，若 null 抛 ParseException，否则用 p.parse(lexer) 并把结果加入 res。
        void parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res) override;

        // 插入新的备选分支
        // 把新的候选放在数组前面（用于动态扩展选择集合）。
        void insert(std::shared_ptr<Parser> p) ;
    };


    // 用途：表示 *（任意次），+（至少一次）或 ?（可选）之类的重复结构；onlyOnce 用于表示仅一次（option）。
    class RepeateElement : public Element {
    public:

        // 解析器Parser
        std::shared_ptr<Parser>  parser;

        // onlyOnce 用于表示仅一次（option）。若 onlyOnce==true，则只做一次（用于 option、maybe 等）。
        bool  onlyOnce;

        // 构造函数
        explicit RepeateElement(std::shared_ptr<Parser>  p, bool once = false);

        // 重载match函数
        bool match(Lexer& lexer) const override;

        // 重载parse解析函数
        void parse(Lexer& lexer, vector<std::shared_ptr<ASTree>>& res) override;
    };


    // 用途：匹配关键词、符号，同时区分那类需要生成 AST 还是仅仅跳过。
    // 叶子节点匹配器（Leaf matcher）  匹配固定文本（例如关键字或运算符文本）
    class Leaf : public Element {
    public:
        // 存储token文本数组 通过 tokens 保存要匹配的字符串数组
        vector<string> tokens;

        // 构造函数
        //new Leaf(new String[]{"if"});  表示这个元素只匹配关键字 "if"
        Leaf(const vector<string>& pat);

        // 重载match函数
        bool match(Lexer& lexer) const override ;


        // 重载parse解析函数， 实际消耗一个 token 并创建 AST 节点
        void parse(Lexer& lexer, vector<std::shared_ptr<ASTree>>& res) override;

        // 将其包装为一个 AST 节点  把匹配到的 token 封装成 AST 节点
        virtual  void find(vector<std::shared_ptr<ASTree>>& res, Token* t) ;
    };


    // Skip
    // 继承自 Leaf，但覆盖 find 为不做任何事情 —— 即消耗该 token
    // 但不把节点加入 AST（用于分隔符、分号、关键字等不想出现在 AST 中的 token）。
    class Skip : public Leaf {
    public:
        // 构造函数
        Skip(const vector<string>& t);

        // 覆盖 find 为不做任何事情
        void find(vector<std::shared_ptr<ASTree>>& res, Token* t) override;
    };

    // ====================== 运算符结合性枚举 ======================
    enum class Associativity {
        LEFT,       // 左结合
        RIGHT,      // 右结合
        NONE        // 无结合
    };

    // ====================== 运算符优先级类 ======================
    class Precedence {
    public:
        int value;                 // 优先级数值（越大优先级越高）
        Associativity assoc;       // 结合性（左、右、无）

        // 默认构造函数
        Precedence(int v = 0, Associativity a = Associativity::NONE);

        // 辅助方法：返回结合性名称
        string assocName() const ;
    };

    // ====================== 运算符优先级表 ======================
    class Operators {
    private:
        map<string, Precedence> table;  // 存储运算符及其优先级

    public:
        // 添加运算符优先级
        void add(const string& op, int prec, Associativity assoc);

        // 获取运算符的优先级
        Precedence* get(const string& op) ;

        // 判断两个运算符的优先级关系
        bool hasHigherPrecedence(const string& op1, const string& op2);

        // 打印优先级表
        void print() const;

        // 构建默认常见运算符优先级表
        static Operators defaultTable();
    };

    /*
    Expr（表达式解析组合子）—— 最关键的部分（优先级/结合性解析）
    Expr 负责把“因子（factor）”与运算符表 ops 结合成正确的二叉（或多叉）表达式 AST。
    它用一个 ops 映射保存每个运算符的 Precedence（优先级值 + 是否左结合），通过 nextOperator 预读运算符并用 doShift 构建以运算符为根的 AST 子树，反复做直到表达式完成。
    Expr 的职责是按运算符优先级与结合性把“因子（factor）”组合成完整表达式 AST
    核心思想是优先级爬升 / precedence climbing（或称为基于优先级的移栈/shift）：
    主要步骤（伪流程）：
    1.right = factor.parse(lexer) —— 先解析左操作数（称之为 right 是因为后面会更新）。
    2.while ((prec = nextOperator(lexer)) != null)：
        (a) 读取当前 operator 的优先级 prec。
        (b) right = doShift(lexer, right, prec.value) —— 用当前 operator 将 right 与后续的子表达式组合成更大的表达式树。

    3.将最终的 right 加入 res。
    doShift(lexer, left, prec) 的大致策略：

    (a) 构造一个临时 list = [ left, new ASTLeaf( lexer.read() ) ]（把 operator token 作为中间节点加入）。
    (b) ASTree right = factor.parse(lexer) —— parse 出右操作数。
    (c) 然后看下一个运算符 next 的优先级：
        (1) 如果下一个运算符存在并且 rightIsExpr(prec, next) 为真（意味着当下优先级允许把 right 继续视为左操作数），则递归或循环继续对 right 做 doShift（实际是让更高优先级的运算符先结合到 right 上）。

    (d) 把 right 加入 list，调用 factory.make(list) 生成一个表达式节点（比如 AddExpr、MulExpr 等）并返回。

    判定 rightIsExpr 的逻辑：
    1. 若下一个运算符是左结合：只有当 prec < nextPrec.value 时，next 应该继续结合到 right（即 next 优先级更高）。
    2. 若是右结合：当 prec <= nextPrec.value 时允许继续（右结合的运算符对等优先级也继续向右）。
    这个判断正是实现运算符优先级与结合性的关键。

    举例（直观）：
    1. 假设 + 优先级 1（左结合）， * 优先级 2（左结合）
    2. 当解析 1 + 2 * 3:
        (a) 解析到 1（left）。
        (b) 看到 +（prec=1），进入 doShift，读 +，解析 right 因子得到 2。
        (c) 看到下一个运算符 *（prec=2），因为 2 的优先级更高（prec < nextPrec），所以 right 会先与 * 3 结合形成 2 * 3，然后再与 1 + (2 * 3) 结合。结果正确体现乘法优先于加法。
    */
    class Expr :public Element {
    public:

        //操作符对象
        Operators ops;

        // 因子解析器
        std::shared_ptr<Parser> factor;   // 改为智能指针

        // 构造函数
        explicit Expr(Parser& exp, Operators& op);

        // 带返回ast类型的构造函数
        template<typename  T>
        explicit Expr(Parser& exp, Operators& op);

        // 重载match方法
        bool match(Lexer& lexer) const override;

        // 重载解析器方法
        void parse(Lexer& lexer, vector<std::shared_ptr<ASTree>>& res);


    private:
        std::shared_ptr<Factory> factory;                // AST 节点工厂

        // 是一个递归下降解析器中处理运算符优先级与结合律的核心函数
        std::shared_ptr<ASTree> doShift(Lexer& lexer, std::shared_ptr<ASTree> left, int prec);

        /*
        判断当前运算符的右边部分是否应继续作为“表达式的一部分”被解析。
        在当前运算符的优先级下，我们是否应该继续向右解析另一个表达式？
        回答的问题="当前的表达式右边的部分是否应该继续被纳入到同一层级的表达式中？"
        工作原理：
        1.取出下一个运算符优先级对象next
        2.比较优先级与结合性：会比较当前运算符与下一个运算符的优先级。
            (a)如果当前运算符的优先级较低【优先爬山】
            OR
            (b)结合方向允许继续【同优先级处理】
            则继续解析右边表达式。



        拓展: 右结合 同级别
            假设当前我们正在解析：
            a = b = c
            语法分析器（Parser）此时看到第一个 =：
            左边是 a
            右边是接下来要解析的部分 b = c
            现在就出现一个关键问题：
            “我们要不要继续往右边解析 b = c？
            还是只解析 b 到此为止？”
            这就是 rightIsExpr() 判断的核心：
            右边是否继续属于一个表达式（即“右边还是表达式吗”）？

            总之，右结合的运算符之所以在同级时也继续解析，是因为我们希望表达式的求值顺序从右往左。

        */
        bool rightIsExpr(int prec, const Precedence& next) const;

        // 获取下一个运算符优先级对象
        Precedence* nextOperator(Lexer& lexer) ;

    };

} // namespace elementChild


#endif //MYPROJECT_ELEMENTCHILD_H