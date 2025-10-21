//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_BINARYEXPR_H
#define MYPROJECT_BINARYEXPR_H

#include "ASTList.h"
#include <initializer_list>

class BinaryExpr: public ASTList
{
public:
    BinaryExpr(vector<std::shared_ptr<ASTree>>& a);

    // 左值
    ASTree *left() const;

    // 操作符
    string operate() const;

    // 右值
    ASTree* right() const;
};


#endif //MYPROJECT_BINARYEXPR_H