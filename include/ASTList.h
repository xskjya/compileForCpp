//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_ASTLIST_H
#define MYPROJECT_ASTLIST_H


#include "Token.h"
#include <sstream>
#include <memory>
#include "ASTree.h"

class ASTList:public ASTree
{
public:
    // 初始化构造函数
    ASTList(const vector<std::shared_ptr<ASTree>> astrees);

    // 析构函数，释放子节点内存
    ~ASTList() =default;
    //virtual允许子类重写该方法，和接口类的设计理念一致
    // 查询第i个节点(ASTree对象: ASTLeaf或ASTList
    std::shared_ptr<ASTree> child(const int i) const override;

    // 返回子节点(即ASTree对象: ASTLeaf或ASTList)的个数(无则返回0
    int numChildren() const override;

    // 获取子节点的迭代对象
    Iterable<std::shared_ptr<ASTree>> childrenIterable() const ;

    // 转化为字符串
    string toString()  override;


    // 定位函数
    std::string location() const override;


private:
    // 存储子节点的迭代对象
    vector<std::shared_ptr<ASTree>> children;

    // Token对象，用于表示操作符等具体内容
    Token* token;
};

#endif //MYPROJECT_ASTLIST_H