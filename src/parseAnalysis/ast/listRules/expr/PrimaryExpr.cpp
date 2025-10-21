//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/PrimaryExpr.h"

PrimaryExpr::PrimaryExpr(const vector<std::shared_ptr<ASTree>>& list) : ASTList(list) {};

// 工厂函数：当只有一个子节点时直接返回该节点，否则返回一个 PrimaryExpr
ASTree* PrimaryExpr::create(std::vector<std::shared_ptr<ASTree> >& c) {
    if (c.size() == 1) {
        return c[0].get();  // 不再包一层
    }
    else {
        return new PrimaryExpr(c);
    }
}
