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
#include "tokenElementChild.h"
#include "Utils.h"
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






    // 构建一个parser对象，静态函数
    static  std::shared_ptr<Parser> rule();
    template<typename clazz=ASTree>
    static  std::shared_ptr<Parser> rule();


    //=================== 初始化规则 ===================
    template<typename ASTClass, typename argtype= vector<std::shared_ptr<ASTree>>>
    std::shared_ptr<Parser> reset();
    // 非模板 reset：仅清空元素，不改变工厂
    std::shared_ptr<Parser> reset();





    //=================== 链式 API（规则构造） ===================

    // 1️⃣ 整数字面量规则
    template<typename leafType = ASTLeaf>  // 模板参数：叶节点类型，默认为 ASTLeaf
    std::shared_ptr<Parser> number() ;

    // 2️⃣ 标识符规则（支持保留字过滤）
    template<typename leafType = ASTLeaf>  // 模板参数：叶节点类型，默认为 ASTLeaf
    std::shared_ptr<Parser> identifier(const std::set<std::string>& reserved) ;

    // 3️⃣ 字符串字面量规则
    template<typename leafType = ASTLeaf>  // 模板参数：叶节点类型，默认为 ASTLeaf
    std::shared_ptr<Parser> str() ;

    // 4️⃣ 匹配指定 token（例如 "+"、"-"）
    std::shared_ptr<Parser> token(const std::vector<std::string>& pat);

    // 5️⃣ 跳过指定 token（例如分号、括号）
    template<typename... Args>
    std::shared_ptr<Parser> sep(Args&&... args);

    // 6️⃣ 嵌套语法规则（子树）
    std::shared_ptr<Parser> ast(std::shared_ptr<Parser>  p) ;

    // 7️⃣ 多选分支（OrTree）
    template<typename... Args>
    std::shared_ptr<Parser> Or(Args&&... args);

    // 8️⃣ 可选元素（出现 0 或 1 次）
    std::shared_ptr<Parser> option(std::shared_ptr<Parser> p);

    // 9️⃣ maybe：可选出现，省略则为空节点
    std::shared_ptr<Parser> maybe(const Parser& p) ;

    // 🔟 表达式规则：由子表达式和运算符表组成
    std::shared_ptr<Parser> expression(Parser &subexp, Operators &operators);
    template<typename T>
    std::shared_ptr<Parser> expression(Parser &subexp, Operators &operators);

    // 11️⃣ 重复多次（0 次或多次）
    std::shared_ptr<Parser> repeat(std::shared_ptr<Parser> p);

    // 12️⃣ 动态插入新的分支（扩展语法）
    std::shared_ptr<Parser> insertChoice(std::shared_ptr<Parser> p);

    //=================== 成员变量 ===================
    std::vector<std::shared_ptr<Element>> elements;  // 当前规则的语法单元序列
    std::shared_ptr<Factory> factory;                // AST 节点工厂
};



// 模板构造函数：初始化为指定 AST 节点类型的解析器
template<typename ASTClass>
inline Parser::Parser() {
    reset<ASTClass>(); // 初始化工厂和清空元素列表
}


//=================== 初始化规则 ===================
// 模板 reset：初始化为指定 AST 类型
template<typename ASTClass, typename argtype>
inline std::shared_ptr<Parser> Parser::reset() {

    // 若是 ASTList 类型，使用专用工厂
    if constexpr (std::is_same_v<ASTClass, ASTList>) {
        factory = Factory::getForASTList();
    }
    // 否则使用模板工厂
    else {
        factory = Factory::get<ASTClass, argtype>();
    }
    elements.clear();             // 清空元素
    return shared_from_this();    // 返回自身（链式调用）
}

// 非模板 reset：仅清空元素，不改变工厂
inline std::shared_ptr<Parser> Parser::reset() {
    elements.clear();
    return shared_from_this();
}

// 带模板参数版本：每次调用都会new一个Parser对象
inline std::shared_ptr<Parser> Parser::rule()
{
    auto p = std::make_shared<Parser>(); // 先让 shared_ptr 拥有对象
    p->reset();                   // 再调 Parser 自己的模板成员
    return p;
}

template<typename clazz>
inline std::shared_ptr<Parser> Parser::rule()
{
    auto p = std::make_shared<Parser>(); // 先让 shared_ptr 拥有对象
    p->reset<clazz>();                   // 再调 Parser 自己的模板成员
    return p;
}

//=================== 添加语法单元 ===================
// 手动向当前规则添加一个 Element
inline  void Parser::addElement(std::shared_ptr<Element> elem) {
    elements.push_back(elem);
}

//=================== 匹配判断 ===================
// 判断输入 token 是否与第一个语法单元匹配
inline bool Parser::match(Lexer& lexer) const {
    if (elements.empty()) return true;       // 空规则默认匹配
    return elements[0]->match(lexer);        // 检查首个元素是否匹配
}

//=================== 主解析函数 ===================
// 顺序执行每个语法单元的 parse()，并将结果组合成 AST
inline std::shared_ptr<ASTree> Parser::parse(Lexer& lexer) {

    cout << "[3] 顺序执行每个语法单元的 parse()，并将结果组合成 AST" << endl;
    std::vector<std::shared_ptr<ASTree>> results;  // 子节点结果容器

    for (int i=0; i < elements.size(); i++) {
        auto e = elements[i];
        printTypeInfo(*e,  "(" + std::to_string(i+1) + ")[Parser::parse] elements");

        e->parse(lexer, results);                  // 执行各语法单元
    }

    cout << "[4] 解析语法单元，并将其构造成AST" << endl;
    return factory->make(&results);                 // 工厂生成 AST 节点
}






//=================== 链式 API（规则构造） ===================
// 1️⃣ 整数字面量规则
template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
inline std::shared_ptr<Parser> Parser::number() {
    elements.push_back(std::make_shared<tokenElementChild::NumTokenElement<leafType>>());  // 匹配整数字面量
    return shared_from_this();
}

// 2️⃣ 标识符规则（支持保留字过滤）
template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
inline std::shared_ptr<Parser> Parser::identifier(const std::set<std::string>& reserved) {
    elements.push_back(std::make_shared<tokenElementChild::IdTokenElement<leafType>>(reserved)); // 匹配标识符
    return shared_from_this();
}


// 3️⃣ 字符串字面量规则
template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
inline std::shared_ptr<Parser> Parser::str() {
    elements.push_back(std::make_shared<tokenElementChild::StrTokenElement<leafType>>());   // 匹配字符串常量
    return shared_from_this();
}


// 4️⃣ 匹配指定 token（例如 "+"、"-"）
inline std::shared_ptr<Parser> Parser::token(const std::vector<std::string>& pat) {
    std::shared_ptr<Element> e = std::make_shared<Leaf>(pat);
    elements.push_back(e);
    return shared_from_this();
}


// 5️⃣ 跳过指定 token（例如分号、括号）
template<typename... Args>
inline std::shared_ptr<Parser> Parser::sep(Args&&... args) {
    // 确保所有实参都是 shared_ptr<Parser> 或可隐式转换
    std::vector<string> pat{ std::forward<Args>(args)... };

    std::shared_ptr<Element> e = std::make_shared<Skip>(pat);
    elements.push_back(e);
    return shared_from_this();
}


// 6️⃣ 嵌套语法规则（子树）
inline std::shared_ptr<Parser> Parser::ast(std::shared_ptr<Parser> p) {
    std::shared_ptr<Element> e = std::make_shared<TreeElement>(p);
    elements.push_back(e);
    return shared_from_this();
}

// 7️⃣ 多选分支（OrTree）
template<typename... Args>
inline std::shared_ptr<Parser> Parser::Or(Args&&... args) {
    // 确保所有实参都是 shared_ptr<Parser> 或可隐式转换
    std::vector<std::shared_ptr<Parser>> parsers{ std::forward<Args>(args)... };

    // 打印调试
    printfOr<Parser>(parsers);

    std::shared_ptr<Element> e = std::make_shared<OrTreeElement>(parsers);
    elements.push_back(e);
    return shared_from_this();
}

// 8️⃣ 可选元素（出现 0 或 1 次）
inline std::shared_ptr<Parser> Parser::option(std::shared_ptr<Parser> p) {
    std::shared_ptr<Element> e = std::make_shared<RepeateElement>(p, true);
    elements.push_back(e);
    return shared_from_this();
}

// 9️⃣ maybe：可选出现，省略则为空节点
inline std::shared_ptr<Parser> Parser::maybe(const Parser& p) {
    Parser p2 = p;
    p2.reset();  // 复制一份空版本

    // 组织一个新的vector
    vector<std::shared_ptr<Parser>> ortree = { std::make_shared<Parser>(p), std::make_shared<Parser>(p2)};

    std::shared_ptr<Element> e = std::make_shared<OrTreeElement>(ortree);
    elements.push_back(e);
    return shared_from_this();
}

// 🔟 表达式规则：由子表达式和运算符表组成
// std::shared_ptr<Parser> Parser::expression(Parser& subexp, Operators& operators) {
//     std::shared_ptr<Element> e = std::make_shared<Expr>(subexp, operators );
//     elements.push_back(e);
//     return shared_from_this();
// }
template<typename T>
inline std::shared_ptr<Parser> Parser::expression(Parser& subexp, Operators& operators) {
    std::shared_ptr<Element> e = std::make_shared<Expr>(subexp, operators );
    elements.push_back(e);
    return shared_from_this();
}

// 11️⃣ 重复多次（0 次或多次）
inline std::shared_ptr<Parser> Parser::repeat(std::shared_ptr<Parser> p) {
    std::shared_ptr<Element> e = std::make_shared<RepeateElement>(p, false);
    elements.push_back(e);
    return shared_from_this();
}








// 12️⃣ 动态插入新的分支（扩展语法）
inline std::shared_ptr<Parser> Parser::insertChoice(std::shared_ptr<Parser> p) {
    // 若首个元素是 OrTree，则直接插入新分支
    if (!elements.empty()) {
        auto first = elements[0];
        auto orTree = std::dynamic_pointer_cast<OrTreeElement>(first);
        if (orTree) {
            orTree->insert(p);
            return shared_from_this();
        }
    }

    // 否则新建 OrTree，将原规则与新分支合并
    Parser otherwise = *this;  // 备份当前规则
    reset();                   // 清空当前规则

    // 构造新的OrTree
    vector<std::shared_ptr<Parser>> newOrTree = { std::make_shared<Parser>(*p) , std::make_shared<Parser>(otherwise)  };
    Or(newOrTree);      // 构造新的 OrTree
    return shared_from_this();
}


#endif //MYPROJECT_PARSER_H