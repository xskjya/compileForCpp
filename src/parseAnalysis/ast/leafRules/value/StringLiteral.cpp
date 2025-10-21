//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/StringLiteral.h"

StringLiteral::StringLiteral(const Token* t) : ASTLeaf(t) {};

// 返回token值
string StringLiteral::value() const {
    return getToken()->getText();
}

