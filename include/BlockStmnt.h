//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_BLOCKSTMNT_H
#define MYPROJECT_BLOCKSTMNT_H


#include "ASTree.h"
#include "ASTList.h"


/*
表示一个 语句块（Block Statement），
例如：
{
    a = 1;
    b = 2;
    print(a + b);
}

在语法树中会表示为：
BlockStmnt([
    BinaryExpr(a, "=", 1),
    BinaryExpr(b, "=", 2),
    PrimaryExpr(print(a + b))
])

即：BlockStmnt 节点下有多个子语句


举例：
std::vector<ASTree*> stmts = { stmt1, stmt2, stmt3 };
BlockStmnt* block = new BlockStmnt(stmts);
*/

class BlockStmnt: public ASTList
{

public:
    /*
    构造函数:
    c 是该语法块内所有子语句的语法树节点集合；
    它直接传递给 ASTList 的构造函数；
    ASTList 内部会保存 children = c
    */
    BlockStmnt(vector<std::shared_ptr<ASTree>>& c);
};



#endif //MYPROJECT_BLOCKSTMNT_H