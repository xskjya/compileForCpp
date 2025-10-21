//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_PARSER_H
#define MYPROJECT_PARSER_H

#include <memory>
#include <vector>
#include <set>
#include <string>
#include "Factory.h"
#include  "elementChild.h"
using namespace  elementChild;


//===================================================================
// Parser 类：基于组合子模式的语法规则构造器
// 用于定义、组合并执行语法规则，生成对应的抽象语法树（AST）。
//===================================================================
class Parser : public std::enable_shared_from_this<Parser> {
public:
    //=================== 构造函数 ===================

    // 模板构造函数：初始化为指定 AST 节点类型的解析器
    template<typename ASTClass>
    Parser();

    Parser();   // 默认构造函数
    ~Parser();  // 默认析构函数

    // 拷贝构造函数：复制另一个 Parser 的元素与工厂
    Parser(const Parser& p);

    //=================== 匹配判断 ===================
    // 判断输入 token 是否与第一个语法单元匹配
    bool match(Lexer& lexer) const;

    //=================== 主解析函数 ===================
    // 顺序执行每个语法单元的 parse()，并将结果组合成 AST
    std::shared_ptr<ASTree> parse(Lexer& lexer);

    //=================== 添加语法单元 ===================
    // 手动向当前规则添加一个 Element
    void addElement(std::shared_ptr<Element> elem);

    //=================== 初始化规则 ===================
    // 模板 reset：初始化为指定 AST 类型
    template<typename ASTClass>
    std::shared_ptr<Parser> reset();

    // 非模板 reset：仅清空元素，不改变工厂
    std::shared_ptr<Parser> reset() ;

    //=================== 链式 API（规则构造） ===================

    // 1️⃣ 整数字面量规则
    template<typename leafType = ASTLeaf>  // 模板参数：叶节点类型，默认为 ASTLeaf
    std::shared_ptr<Parser> number() ;

    // 2️⃣ 标识符规则（支持保留字过滤）
    template<typename leafType = ASTLeaf>  // 模板参数：叶节点类型，默认为 ASTLeaf
    std::shared_ptr<Parser> identifier(const std::set<std::string>& reserved) ;


    // 3️⃣ 字符串字面量规则
    template<typename leafType = ASTLeaf>  // 模板参数：叶节点类型，默认为 ASTLeaf
    std::shared_ptr<Parser> string() ;


    // 4️⃣ 匹配指定 token（例如 "+"、"-"）
    std::shared_ptr<Parser> token(const std::vector<std::string>& pat);


    // 5️⃣ 跳过指定 token（例如分号、括号）
    std::shared_ptr<Parser> sep(const std::vector<std::string>& pat);

    // 6️⃣ 嵌套语法规则（子树）
    std::shared_ptr<Parser> ast(std::shared_ptr<Parser>  p) ;


    // 7️⃣ 多选分支（OrTree）
    std::shared_ptr<Parser> Or(vector<std::shared_ptr<Parser>>& parsers);

    // 8️⃣ 可选元素（出现 0 或 1 次）
    std::shared_ptr<Parser> option(std::shared_ptr<Parser> p);

    // 9️⃣ maybe：可选出现，省略则为空节点
    std::shared_ptr<Parser> maybe(const Parser& p) ;

    // 🔟 表达式规则：由子表达式和运算符表组成
    std::shared_ptr<Parser> expression(Parser &subexp, Operators &operators) ;

    // 11️⃣ 重复多次（0 次或多次）
    std::shared_ptr<Parser> repeat(std::shared_ptr<Parser> p);

    // 12️⃣ 动态插入新的分支（扩展语法）
    std::shared_ptr<Parser> insertChoice(std::shared_ptr<Parser> p);

    //=================== 成员变量 ===================
    std::vector<std::shared_ptr<Element>> elements;  // 当前规则的语法单元序列
    std::shared_ptr<Factory> factory;                // AST 节点工厂
};



#endif //MYPROJECT_PARSER_H