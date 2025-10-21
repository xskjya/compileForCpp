//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_NUMBERLITERAL_H
#define MYPROJECT_NUMBERLITERAL_H


#include "ASTLeaf.h"


class NumberLiteral : public ASTLeaf {
public:
    NumberLiteral(const Token* t) ;

    // 获取数字的值
    signed int value() const;
};


#endif //MYPROJECT_NUMBERLITERAL_H