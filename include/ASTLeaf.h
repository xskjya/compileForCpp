//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_ASTLEAF_H
#define MYPROJECT_ASTLEAF_H

#include "ASTree.h"
#include "Token.h"


// 属于ASTree的子类，属于AStTree的叶节点的不同类型节点类型——叶节点
class ASTLeaf: public ASTree {
public:
    // 初始化构造函数, 默认构造叶节点的迭代对象为空
    ASTLeaf(const Token* t);

    // 查询第i个节点(ASTree对象: ASTLeaf或ASTList
    std::shared_ptr<ASTree> child(const int i) const override;

    // 返回子节点(即ASTree对象: ASTLeaf或ASTList)的个数(无则返回0
    int  numChildren() const override ;

    // 将返回一个用于表示抽象语法树节点在程序内所处位置的字符串
    string location() const override;

    // 返回叶节点的具体内容
    string toString()  override;

    // 返回叶节点的Token对象
    const Token* getToken() const;
protected:
    // Token对象，表示叶节点的具体内容
    const Token*  token;
};


#endif //MYPROJECT_ASTLEAF_H