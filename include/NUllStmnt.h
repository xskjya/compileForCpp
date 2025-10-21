//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_NULLSTMNT_H
#define MYPROJECT_NULLSTMNT_H

#include "ASTList.h"

class NUllStmnt : public  ASTList
{

public:
    // 构造函数
    NUllStmnt(vector<std::shared_ptr<ASTree>>& c);
};


#endif //MYPROJECT_NULLSTMNT_H