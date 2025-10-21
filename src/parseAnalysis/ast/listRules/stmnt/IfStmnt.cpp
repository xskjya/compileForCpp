//
// Created by xskj on 2025/10/18.
//

#include "IfStmnt.h"


IfStmnt::IfStmnt(std::vector<std::shared_ptr<ASTree>>& c) : ASTList(c) {}

ASTree* IfStmnt::condation() {
    return child(0).get();
}

ASTree* IfStmnt::thenBlock() {
    return child(1).get();
}

ASTree* IfStmnt::elseBlock() {
    return (numChildren() > 2 ? child(2) : nullptr).get();
}

std::string IfStmnt::toString() {
    return "(if " + condation()->toString() + " " + thenBlock()->toString()
         + " else " + (elseBlock() ? elseBlock()->toString() : "null") + ")";
}