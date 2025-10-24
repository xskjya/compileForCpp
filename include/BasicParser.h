//
// Created by xskj on 2025/10/21.
//

#ifndef MYPROJECT_BASICPARSER_H
#define MYPROJECT_BASICPARSER_H
#include "ASTree.h"
#include "BinaryExpr.h"
#include "BlockStmnt.h"
#include "Lexer.h"
#include "Name.h"
#include "NegativeExpr.h"
#include "NUllStmnt.h"
#include "NumberLiteral.h"
#include "Parser.h"
#include "PrimaryExpr.h"
#include "StringLiteral.h"


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


class BasicParser {
public:
    // 存储标识符的关键词Map
    std::set<std::string> reserved; // 静态共享保留字表

    // 操作符对象
    Operators operators =  Operators();
    // 抽象语法规则对象
    std::shared_ptr<Parser>  rule();
    template<typename T>
    std::shared_ptr<Parser>  rule();


    // 构造函数：初始化标识符关键字
    BasicParser();

    // 解析接口: lexer -》 Parser => ast
    std::shared_ptr<ASTree> parse(Lexer& lexer);

    /****************语法语素构成: 都封装成Parser类进行传递处理********************/
    /*
     * 0. 初始化为无类型工厂的expr
     */
     std::shared_ptr<Parser> expr0 = rule();

    /*
     * 1. 最小终结符语素primary: 原始 => 表达式 OR 字面量 OR 标志符 OR 字符
    *     定义基于非终结符primary的语法规则
     * BNF:
     *      primary  : "("expr")"|NUMBER|IDENTIFIER|STRING
     */
    std::shared_ptr<Parser> primary = rule<PrimaryExpr>()->Or(
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
    std::shared_ptr<Parser> factor = rule()->Or(
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
    std::shared_ptr<Parser> expr = expr0->expression<BinaryExpr>(*factor, operators);

    /*
     * 初始化声明Parser对象
     */
    std::shared_ptr<Parser> statement0 = rule();

    /*
     * 4. 块语素block:
     * BNF:
     *     block　　:  "{"[statement]{(";"|EOL)[statement]}"}"　
     */
    std::shared_ptr<Parser> block = rule<BlockStmnt>()->sep("{")                    // "{"
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
    std::shared_ptr<Parser> simple = rule<PrimaryExpr>()  // 首先定义一个具有主表达式PrimaryExpr工厂类型的Parser解析器
                                        ->ast(expr);      // 包装expr表达式

    /*
     * 6. 声明语素statement:
     * BNF:
     *     statement　:"if" expr block ["else" block]
                     |"while" expr block
                     |simple
     */
    std::shared_ptr<Parser> statement = statement0->Or(       // 语义化定义
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
    std::shared_ptr<Parser> program = rule()->option(statement)->sep(";", Token::EoL);
    // std::shared_ptr<Parser> program = rule()->Or(statement, rule<NUllStmnt>())->sep(";", Token::EoL);

};



// 规则化语法规则对象
inline  std::shared_ptr<Parser>  BasicParser::rule() {
 return   std::make_shared<Parser>(Parser::rule()) ;
}
template<typename T>
inline std::shared_ptr<Parser>  BasicParser::rule() {
 return  std::make_shared<Parser>(Parser::rule<T>());
}

// 解析接口: lexer -》 Parser => ast
inline std::shared_ptr<ASTree> BasicParser::parse(Lexer& lexer) {
 return  program->parse(lexer);
};


#endif
