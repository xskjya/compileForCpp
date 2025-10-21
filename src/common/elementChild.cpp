//
// Created by xskj on 2025/10/19.
//
#include "../../include/elementChild.h"
#include <iostream>
#include "ASTList.h"
#include "Parser.h"          // 这里拿到 Parser 完整定义
using namespace elementChild;



/*************************派生类：具体语法元素实现*********************************/

// ---------- TreeElement ----------
//TreeElement: 解析一个 ASTree 节点,   接收一个 Parser 对象
//用途：当某一部分语法本身又由一个子 Parser 定义（例如一个语句块或子表达式），就用 Tree 来嵌套。

// 构造函数   引用，取别名
TreeElement::TreeElement(const std::shared_ptr<Parser>& p) : parser(p) {}

// 重载match函数
bool TreeElement::match(Lexer& lexer) const{
    // 直接委托给内部 parser.match(lexer)。
    return parser->match(lexer);
}

// 重载parse解析函数
void TreeElement::parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res) {
    // 调用 parser.parse(lexer) 得到一个 ASTree（子树）并把它 add 到 res。
    res.push_back(parser->parse(lexer));
}



// ---------- OrTreeElement ----------
// 用途：表示语法中 “A | B | C” 这样的选择。insert 支持在已有选择前插入优先级更高的分支（常用于语法构造的动态修改）。
// 构造函数
OrTreeElement::OrTreeElement(std::vector<std::shared_ptr<Parser>>& p) : parsers(p){}

// 重载match函数
bool OrTreeElement::match(Lexer& lexer) const {
// 依次对每个 Parser 调用 p.match(lexer)，第一个返回 true 的就是被选择的分支（预测阶段）。
for (auto p : parsers) {
    // 匹配
    try {
        if (p->match(lexer)) return true;
    }
    catch (ParseException& e) {
        // 忽略异常，继续尝试下一个分支
        continue;
    }
}
return false;
}


// 选择匹配的parser进行解析
std::shared_ptr<Parser>  OrTreeElement::choose(Lexer& lexer) const {
    // 返回第一个匹配的 Parser（或 null）。
    for (auto p : parsers) {
        try {
            if (p->match(lexer)) return p;
        }
        catch (ParseException& e) {
            // 忽略异常，继续尝试下一个分支
            continue;
        }
    }
    return nullptr;
}


// 重载parse解析函数
// 调用 choose 获取具体分支 p，若 null 抛 ParseException，否则用 p.parse(lexer) 并把结果加入 res。
void OrTreeElement::parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res) {

    // 调用获取分支Parser
    std::shared_ptr<Parser>  p = choose(lexer);

    if (p) {
        // 匹配成功，解析
        res.push_back(p->parse(lexer));
    }

    // 没匹配的分支，抛异常
    throw ParseException("choose function no match found!");
}

// 插入新的备选分支
// 把新的候选放在数组前面（用于动态扩展选择集合）。
void OrTreeElement::insert(std::shared_ptr<Parser>  p) {
    parsers.insert(parsers.begin(), p);
}

// ---------- RepeateElement ----------
// 用途：表示 *（任意次），+（至少一次）或 ?（可选）之类的重复结构；onlyOnce 用于表示仅一次（option）。
RepeateElement::RepeateElement(std::shared_ptr<Parser>  p, bool once) : parser(p), onlyOnce(once) {};

// 重载match函数
bool RepeateElement::match(Lexer& lexer) const  {
    // 直接委托给内部 parser.match(lexer)。
    return parser->match(lexer);
}

// 重载parse解析函数
void RepeateElement::parse(Lexer& lexer, vector<std::shared_ptr<ASTree>>& res)  {

    // 尝试匹配若干次
    while (parser->match(lexer)) { // 指针移动 循环匹配token结果是否匹配

        // 执行解析
        std::shared_ptr<ASTree> t = parser->parse(lexer);

        // 跳过空的：跳过空 ASTList 是为了在解析可重复结构（如 *、+、?）时，避免在语法树中留下空分支；而 ASTLeaf 永远是实际的 token，不存在“空”的概念，所以无需判断
        std::shared_ptr<ASTList> list = std::dynamic_pointer_cast<ASTList>(t); // 基类转化为子类

        if (list && list->numChildren() == 0) continue;

        // 添加到结果res
        res.push_back(t);

        // 判断是否为onlyOnce
        if (onlyOnce) break; // 只做一次（用于 option、maybe 等）
    }
}



// ---------- ATokenElement ----------
// ATokenElement: 解析一个特定类型的 Token（如标识符、数字、字符串等）；用于匹配具体的 TokenType。
// 基于 token 的原子解析 抽象类，集中处理“从 Lexer 读一个 token 并根据 test(Token t) 判断是否符合”的模式：
// 用途：匹配标识符、数字、字符串字面量等原子 token。
// 是一种 “原子语法元素”（atomic element）——在解析器组合子结构中负责匹配并生成 单个叶子节点（ASTLeaf）
// ASTLeaf的子类为NumberLiteral和Name，分别用于匹配数字和标识符
// 默认为 ASTLeaf 节点，但可通过工厂函数定制为其他类型的叶节点（如 NumberLiteral、Name 等）。
//数字 token → NumberLiteral.class
//标识符 token → Name.class
//默认情况 → ASTLeaf.class
template<typename leafType>
ATokenElement<leafType>::ATokenElement() {
    static_assert(std::is_base_of<ASTLeaf, leafType>::value, "LeafType must derive from ASTLeaf");

    // 获取工厂对象实例
    factory = Factory::get<leafType, Token>();
}

template<typename leafType>
bool ATokenElement<leafType>::match(Lexer& lexer) const  {
    // 取下一个 token（但不消费它），若 null 则返回 false，否则调用 test(t)。
    Token* t = lexer.peek(0);        // 查看下一个token
    if (t == nullptr) return false; // 没有token
    return test(*t); // 测试token类型
}


template<typename leafType>
void ATokenElement<leafType>::parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res)   {

    // 读取token
    Token* t = lexer.read();

    // 匹配token
    if (test(*t)) {
        // 利用工厂函数创建叶节点ASTLeaf对象
        std::shared_ptr<ASTree> leaf = factory->make(t);

        // 添加进结果res
        res.push_back(leaf);
    }
    else {
        // token类型不匹配，抛异常
        throw ParseException(t->getText() + " type mismatch");
    }
}





// ---------- Leaf / Skip ----------

// 用途：匹配关键词、符号，同时区分那类需要生成 AST 还是仅仅跳过。
// 叶子节点匹配器（Leaf matcher）  匹配固定文本（例如关键字或运算符文本）
Leaf::Leaf(const vector<string>& pat) : tokens(pat) {}

// 重载match函数
bool Leaf::match(Lexer& lexer) const  {
    Token* t = lexer.peek(0); // 查看下一个token
    if (t->isIdentifier()) {
        // 标识符类型  循环tokens
        for (string token : tokens) {
            if (token == t->getText()) return true; // 匹配成功
        }
    }
    return false; // 没有匹配成功
}

// 重载parse解析函数， 实际消耗一个 token 并创建 AST 节点
void Leaf::parse(Lexer& lexer, vector<std::shared_ptr<ASTree>>& res)  {
    // 消费一个token
    Token* t = lexer.read();

    // 判断是否匹配
    if (t->isIdentifier()) {

        // 遍历token匹配
        for (string token : tokens) {
            // 匹配
            if (token == t->getText()) {
                // 将其包装为一个 AST 节点
                find(res, t);
                return;
            }
        }
    }

    // // 如果没有匹配到，说明解析失败
    if (tokens.size() > 0)
        throw ParseException(tokens[0] + " expected.", *t);
    else
        throw ParseException("tokens is empty!", *t);


}

// 将其包装为一个 AST 节点  把匹配到的 token 封装成 AST 节点
void Leaf::find(vector<std::shared_ptr<ASTree>>& res, Token* t) {
        std::shared_ptr<ASTLeaf> leaf = std::make_shared<ASTLeaf>(t);
        res.push_back(leaf);
    }

// Skip
// 继承自 Leaf，但覆盖 find 为不做任何事情 —— 即消耗该 token
// 但不把节点加入 AST（用于分隔符、分号、关键字等不想出现在 AST 中的 token）。
// 构造函数
Skip::Skip(const vector<string>& t) : Leaf(t) {};

// 覆盖 find 为不做任何事情
void Skip::find(vector<std::shared_ptr<ASTree>>& res, Token* t)  {
    // 什么都不做，跳过该 token
}



// ---------- Precedence ----------
// ====================== 运算符优先级类 ======================
// 默认构造函数
Precedence::Precedence(int v, Associativity a) : value(v), assoc(a) {}

// 辅助方法：返回结合性名称
string Precedence::assocName() const {
    switch (assoc) {
    case Associativity::LEFT:  return "LEFT";
    case Associativity::RIGHT: return "RIGHT";
    case Associativity::NONE:  return "NONE";
    }
    return "UNKNOWN";
}




// ====================== 运算符优先级表 ======================

// 添加运算符优先级
void Operators::add(const string& op, int prec, Associativity assoc) {
    table[op] = Precedence(prec, assoc);
}

// 获取运算符的优先级
Precedence* Operators::get(const string& op) {
    auto it = table.find(op);
    if (it != table.end())
        return &(it->second);
    return nullptr; // 若未找到，返回空指针
}

// 判断两个运算符的优先级关系
bool Operators::hasHigherPrecedence(const string& op1, const string& op2) {
    const Precedence* p1 = get(op1);
    const Precedence* p2 = get(op2);
    if (!p1 || !p2) return false;
    return p1->value > p2->value;
}

// 打印优先级表
void Operators::print() const {
    cout << left << setw(10) << "Operator"
        << setw(10) << "Precedence"
        << setw(12) << "Associativity" << endl;
    cout << string(32, '-') << endl;

    for (const auto& kv : table) {
        cout << left << setw(10) << kv.first
            << setw(10) << kv.second.value
            << setw(12) << kv.second.assocName()
            << endl;
    }
}

// 构建默认常见运算符优先级表
Operators Operators::defaultTable() {
    Operators ops;
    ops.add("+", 10, Associativity::LEFT);
    ops.add("-", 10, Associativity::LEFT);
    ops.add("*", 20, Associativity::LEFT);
    ops.add("/", 20, Associativity::LEFT);
    ops.add("^", 30, Associativity::RIGHT);
    ops.add("=", 5, Associativity::RIGHT);
    return ops;
}



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
2. 当解析 1 + 2 * 3：
    (a) 解析到 1（left）。
    (b) 看到 +（prec=1），进入 doShift，读 +，解析 right 因子得到 2。
    (c) 看到下一个运算符 *（prec=2），因为 2 的优先级更高（prec < nextPrec），所以 right 会先与 * 3 结合形成 2 * 3，然后再与 1 + (2 * 3) 结合。结果正确体现乘法优先于加法。
*/
Expr::Expr(Operators& op, Parser& exp):ops(op), factor(std::make_shared<Parser>(exp) )  // 这里直接调用拷贝构造函数
    {
		// 创建一个List的ASTList工厂 ，用于自动生成ASTList节点
        factory = Factory::getForASTList();
    }


// 重载match方法
bool Expr::match(Lexer& lexer) const  {
    // 调用解析器中的匹配方法
    return  factor->match(lexer);
};

// 重载解析器方法
void Expr::parse(Lexer& lexer, vector<std::shared_ptr<ASTree>>& res) {

    // 首先获取左操作数ASTree对象  将left左操作数对象->形式上看做右操作数  这样做的目的是，让代码在形式上感觉按token顺序进行逐个解析的 right的传递，添加，更新
    std::shared_ptr<ASTree> right = factor->parse(lexer);  //这里可以看做是在对因子进行解析，其内部采用的优先爬山算法


    // 实例化一个优先级对象
    Precedence* prec;

    // 循环表达式expr的操作符： 操作符驱动
    while ((prec = nextOperator(lexer)) != nullptr) {    // 跟新token并获取相应的优先级对象
        // 遍历对象为token，按照lexer逐步读取token进行遍历该Expr的所有token
        // 调用递归计算表达式方法,传入函数的实际为形式化的右操作数，实际值为做操作数，这样做目的为保持递归参数的一致性
        right = doShift(lexer, right, prec->value); // 函数返回的right才为实际有操作数结果
    }

    // 添加进AST资源数组中
    res.push_back(right);
}

// 是一个递归下降解析器中处理运算符优先级与结合律的核心函数
std::shared_ptr<ASTree> Expr::doShift(Lexer& lexer, std::shared_ptr<ASTree> left, int prec)  {

    // 构造一个ASTree数组存储vector
    vector<std::shared_ptr<ASTree>> list;

    // 增加做操作数                +左操作数
    list.push_back(left);

    // 消费一个token增加一个操作符   +操作符
    list.push_back(std::make_shared<ASTLeaf>(lexer.read()));

    // 解析右操作数
    std::shared_ptr<ASTree> right = factor->parse(lexer);

    // g构造下一个next优先级对象
    Precedence* next;

    // 递归算法循环遍历：遍历token 判断条件， 当优先级
    while (
        (next = nextOperator(lexer)) != nullptr             // 遍历token并获取操作符，且存在， 不进行消费
        &&
        rightIsExpr(prec, *next)                            //右操作数是否可以继续作为表达式的一部分解析
        ) {
        // 自我调用函数实现递归 递归更新right  传入需要更新的right并更新自己
        right = doShift(lexer, right, next->value);
    }

    // 将其添加进list中         +右操作数
    list.push_back(right);

    // 通过工厂模式创建 ASTList 对象返回
    std::shared_ptr<ASTree> ast = factory->make(&list);                 // 工厂生成 AST 节点

    // 如果 list.size() == 1 返回单节点，否则返回 ASTList
    return ast;   // 创建ASTList方法
}

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
bool Expr::rightIsExpr(int prec, const Precedence& next) const {
    switch (next.assoc) {
    case Associativity::LEFT:
        // 左结合：仅当右侧优先级更高时才向右递归
        return prec < next.value;
    case Associativity::RIGHT:
        // 右结合：右侧优先级相等或更高都继续递归
        return prec <= next.value;
    case Associativity::NONE:
    default:
        // 无结合：不允许相同级别连写
        if (prec == next.value) {
            throw ParseException("Non-associative operator used multiple times at same precedence.");
        }
        return prec < next.value;  // 只允许更高优先级的操作继续
    }
}

// 获取下一个运算符优先级对象
Precedence* Expr::nextOperator(Lexer& lexer) {
    // 预读token
    Token* t = lexer.peek(0);
    if (!t) return nullptr;
    return ops.get(t->getText());
}



// 命名空间生命
namespace elementChild {
    template class ATokenElement<ASTLeaf>;
}
