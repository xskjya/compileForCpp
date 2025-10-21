//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/Name.h"

Name::Name(const Token* t) : ASTLeaf(t) {}

std::string Name::name() const {
    return getToken()->getText();   // 直接从 Token 拿文本
}