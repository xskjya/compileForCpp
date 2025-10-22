//
// Created by xskj on 2025/10/22.
//

#include "../../../include/ParserRunner.h"
#include <iostream>
#include "Lexer.h"
#include  "BasicParser.h"
#include  "Token.h"


void ParserRunner::main() {

        // 获取词法分析器对象
        std::string filename = "../assets/test.cpp";
        Reader reader(filename);
        Lexer lexer(reader);

        // 获取基础语法分析对象
        BasicParser bp = BasicParser();

        // 对Lexer词法分析器中的token进行循环遍历进行语法分析处理，生成AST
        while (lexer.peek(0)->getText() != Token::EoF) {  // 判断是否读取到程序结束
            // 获取加载的token的抽象语法树ast, 利用parse接口进行语法树解析
            ASTree ast = bp.parse(lexer);

            // 打印出抽象语法树
            cout << "==> " << ast.toString() << endl;
        }

    }

