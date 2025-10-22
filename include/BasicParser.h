//
// Created by xskj on 2025/10/21.
//

#ifndef MYPROJECT_BASICPARSER_H
#define MYPROJECT_BASICPARSER_H
#include "ASTree.h"
#include "Lexer.h"


// 对Parser类的进一步应用化利用封装

class BasicParser {

public:
    BasicParser();

    // 解析接口: lexer -》 Parser => ast
    ASTree  parse(Lexer& lexer);
};


#endif //MYPROJECT_BASICPARSER_H