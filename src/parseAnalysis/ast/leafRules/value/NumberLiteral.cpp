//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/NumberLiteral.h"

NumberLiteral::NumberLiteral(const Token* t) : ASTLeaf(t) {};
// 获取数字的值
signed int NumberLiteral::value() const {
    return getToken()->getNumber();
}


