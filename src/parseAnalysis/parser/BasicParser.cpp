//
// Created by xskj on 2025/10/21.
//

#include "../../../include/BasicParser.h"



// 对Parser类的进一步应用化利用封装
/*
 *匹配一条完整语句语法的封装类：
 *如下：
primary  : "("expr")"|NUMBER|IDENTIFIER|STRING
factor　 : "-"primary|primary
expr    :  factor{OP factor}
block　　:  "{"[statement]{(";"|EOL)[statement]}"}"　
simple　 :  expr　
statement　:"if" expr block ["else" block]
             |"while" exprblock
             |simple
program　:[ statement](";"|EOL)
 */


// 构造函数：初始化标识符关键字
BasicParser::BasicParser() {

    cout << "[0] 初始化标识符的关键词Map reserved和操作符operators" << endl;

    // 存储标识符的关键词Map
    reserved = set<string>{}; // 静态共享保留字表

    // 操作符对象
    operators = Operators();

    // 新增标识符关键字符
    reserved.insert(";");
    reserved.insert("}");
    reserved.insert(Token::EoL);

    // 新增操作符
    // 左结合
    operators.add("=", 1,Associativity::RIGHT);
    operators.add("==", 2,Associativity::LEFT);
    operators.add(">", 2,Associativity::LEFT);
    operators.add("<", 2,Associativity::LEFT);

    // 右结合
    operators.add("+", 3,Associativity::RIGHT);
    operators.add("-", 3,Associativity::RIGHT);
    operators.add("*", 4,Associativity::RIGHT);
    operators.add("/", 4,Associativity::RIGHT);
    operators.add("%", 4,Associativity::RIGHT);


    cout << "[1] 语法语素构成.....  都封装成Parser类进行传递处理" << endl;
    /****************语法语素构成: 都封装成Parser类进行传递处理********************/
    /*
     * 0. 初始化为无类型工厂的expr
     */
    expr0 = rule();

    /*
     * 1. 最小终结符语素primary: 原始 => 表达式 OR 字面量 OR 标志符 OR 字符
    *     定义基于非终结符primary的语法规则
     * BNF:
     *      primary  : "("expr")"|NUMBER|IDENTIFIER|STRING
     */
    primary = rule<PrimaryExpr>()->Or(
            // 含括号地表达式
           rule()->sep("(")->ast(expr0)->sep(")"),

           // 字面量
           rule()->number<NumberLiteral>(),

           // 标识符
           rule()->identifier<Name>(reserved),

           // 字符串
           rule()->str<StringLiteral>()
    );

    /*
     * 2. 因子语素factor:
     * BNF:
     *     factor　 : "-"primary|primary
     */
    factor = rule()->Or(
          // "-"primary
          rule<NegativeExpr>()->sep("-")->ast(primary),
          // primary
          primary
    );

    /*
     * 3. 表达式语素expr:
     * BNF:
     *     expr    :  factor {OP  factor}  抽象出两个参数: Parser对象factor 和 操作符对象OP
     */
    expr = expr0->expression<BinaryExpr>(*factor, operators);

    /*
     * 初始化声明Parser对象
     */
    statement0 = rule();

    /*
     * 4. 块语素block:
     * BNF:
     *     block　　:  "{"[statement]{(";"|EOL)[statement]}"}"　
     */
    block = rule<BlockStmnt>()->sep("{")                    // "{"
                              ->option(statement0)          // [statement]
                              ->repeat(                     // {
                               rule()->sep(";", Token::EoL) //   (";"|EOL)
                                     ->option(statement0)   //   [statement]
                               )                            // }
                              ->sep("}");                   // "}"

    /*
     * 5. 简单语素simple: 主要为主表达式的表达
     * BNF:
     *     simple　 :  expr　
     * 提示： 这里不直接用: std::shared_ptr<Parser> simple = expr; 主要是因为了在语法表述层面将该simple语法的语义化，增强代码的语义化代码描述
     */
    simple = rule<PrimaryExpr>()  // 首先定义一个具有主表达式PrimaryExpr工厂类型的Parser解析器
                                        ->ast(expr);      // 包装expr表达式

    /*
     * 6. 声明语素statement:
     * BNF:
     *     statement　:"if" expr block ["else" block]
                     |"while" expr block
                     |simple
     */
    statement = statement0->Or(       // 语义化定义
                                             rule()->sep("if")   // "if"
                                              ->ast(expr)   // expr
                                              ->ast(block)   // block
                                              ->option(
                                               rule()->sep("else")->ast(block)
                                               ),               // ]
                                             rule()->sep("while")->ast(expr)->ast(block),     // "while" expr block
                                             simple                               // simple
                                           );
    /*
     * 7. 完整程序语法语句语素program:
     * BNF:
           program　:[ statement](";"|EOL)
    */
    program = rule()->option(statement)->sep(";", Token::EoL);
    // program = rule()->Or(statement, rule<NUllStmnt>())->sep(";", Token::EoL);

}

// 规则化语法规则对象
std::shared_ptr<Parser>  BasicParser::rule() {
    return   Parser::rule();
}

template<typename T>
std::shared_ptr<Parser>  BasicParser::rule() {
 return  Parser::rule<T>();
}


// 解析接口: lexer -》 Parser => ast
std::shared_ptr<ASTree> BasicParser::parse(Lexer& lexer) {
    // 调试
    cout << "[2] 调用语法语句program解析......" << endl;
    return  program->parse(lexer);
};

