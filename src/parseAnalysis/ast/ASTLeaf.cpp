//
// Created by xskj on 2025/10/18.
//

#include "../../../include/ASTLeaf.h"


// 属于ASTree的子类，属于AStTree的叶节点的不同类型节点类型——叶节点

// 初始化构造函数, 默认构造叶节点的迭代对象为空
ASTLeaf::ASTLeaf(const Token* t):ASTree(),token(t) {};

// 查询第i个节点(ASTree对象: ASTLeaf或ASTList
std::shared_ptr<ASTree> ASTLeaf::child(const int i) const {
    throw  runtime_error("no child");  // 叶节点没有子节点
};

// 返回子节点(即ASTree对象: ASTLeaf或ASTList)的个数(无则返回0
int  ASTLeaf::numChildren() const {return 0;};

// 将返回一个用于表示抽象语法树节点在程序内所处位置的字符串
string ASTLeaf::location() const {
    return "at line:" + token->getLineNo();  // 这里可以换成行号等信息
};

// 返回叶节点的具体内容
string ASTLeaf::toString() {
    return token->getText();
}

// 返回叶节点的Token对象
const Token* ASTLeaf::getToken() const {
    return token;
}

