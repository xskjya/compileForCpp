//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_ASTREE_H
#define MYPROJECT_ASTREE_H

#include <string>
#include "Iterable.h"
#include <memory>
using namespace std;

// 接口类
class ASTree: public Iterable<ASTree*> {
public:
    // 初始化构造函数
    ASTree(const vector<std::shared_ptr<ASTree>>& astrees);

    // 默认构造函数
    ASTree() ;

    //virtual允许子类重写该方法，和接口类的设计理念一致
    // 查询第i个节点(ASTree对象: ASTLeaf或ASTList
    virtual std::shared_ptr<ASTree> child(const int i) const;

    // 返回子节点(即ASTree对象: ASTLeaf或ASTList)的个数(无则返回0
    virtual int numChildren() const;

    // 转化为字符串
    virtual std::string toString();

    // 将返回一个用于表示抽象语法树节点在程序内所处位置的字符串
    virtual string location() const;

    // 返回子节点迭代对象
    Iterable<std::shared_ptr<ASTree> > iterable() const;

private:

    //返回一个用于遍历子节点的迭代对象
    Iterable<std::shared_ptr<ASTree>>  children;
};


#endif //MYPROJECT_ASTREE_H