//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_WHILESTMNT_H
#define MYPROJECT_WHILESTMNT_H


#include "ASTList.h"
#include "ASTree.h"

class WhileStmnt: public ASTList
{

public:

    WhileStmnt(vector<std::shared_ptr<ASTree>>& c);

    // 判断条件
    ASTree* condition();

    // 循环体
    ASTree* body();

    // 声明虚函数重载（在这里使用 override）
    std::string toString()  override;
};


#endif //MYPROJECT_WHILESTMNT_H