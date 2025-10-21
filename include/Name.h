//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_NAME_H
#define MYPROJECT_NAME_H


#include "ASTLeaf.h"


class Name: public ASTLeaf
{
public:
    Name(const Token* t);

    // 获取标识符的名字
    string name() const;
};


#endif //MYPROJECT_NAME_H