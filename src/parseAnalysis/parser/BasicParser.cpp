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
}

