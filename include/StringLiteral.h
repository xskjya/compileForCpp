//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_STRINGLITERAL_H
#define MYPROJECT_STRINGLITERAL_H


#include "ASTLeaf.h"
#include "Token.h"


class StringLiteral: public ASTLeaf
{
public:
    StringLiteral(const Token* t) ;

    // 获取值
    string value() const;
};

#endif //MYPROJECT_STRINGLITERAL_H