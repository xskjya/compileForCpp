//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/NegativeExpr.h"


NegativeExpr::NegativeExpr(std::vector<std::shared_ptr<ASTree>>& c)
    : ASTList(c) {}

std::shared_ptr<ASTree> NegativeExpr::operand() {
    return child(0);   // 一元负号只有一个子节点
}

std::string NegativeExpr::toString() {
    return "-" + operand()->toString();
}