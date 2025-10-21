//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/BinaryExpr.h"

#include "ASTLeaf.h"


// 左值
ASTree* BinaryExpr::left() const {
    return child(0).get();
};

// 操作符
string BinaryExpr::operate() const {
    ASTLeaf* leaf = dynamic_cast<ASTLeaf*>(child(1).get());
    if (leaf) {
        return leaf->getToken()->getText();
    }
    return "";
}

// 右值
ASTree* BinaryExpr::right() const {
    return  child(2).get();
}