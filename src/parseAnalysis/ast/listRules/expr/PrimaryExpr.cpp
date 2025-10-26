//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/PrimaryExpr.h"

PrimaryExpr::PrimaryExpr(const vector<std::shared_ptr<ASTree>>& list) : ASTList(list) {};


// 工厂函数：当只有一个子节点时直接返回该节点，否则返回一个 PrimaryExpr
std::shared_ptr<ASTree> PrimaryExpr::create(std::vector<std::shared_ptr<ASTree>>& c) {
    if (c.size() == 1) {
        return c[0];                          // 直接返回已有的 shared_ptr
    } else {
        return std::make_shared<PrimaryExpr>(c); // 新的 shared_ptr
    }
}
