//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_PRIMARYEXPR_H
#define MYPROJECT_PRIMARYEXPR_H


#include "ASTList.h"


// 它用于表示最基础的“主表达式（Primary Expression）”，例如字面量、变量、括号表达式等。
class PrimaryExpr: public ASTList
{
public:
    PrimaryExpr(const vector<std::shared_ptr<ASTree>>& list);

    // 工厂函数：当只有一个子节点时直接返回该节点，否则返回一个 PrimaryExpr
    static ASTree* create(std::vector<std::shared_ptr<ASTree>> &c);
};



#endif //MYPROJECT_PRIMARYEXPR_H