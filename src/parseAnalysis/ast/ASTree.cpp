//
// Created by xskj on 2025/10/18.
//

#include "../../../include/ASTree.h"
#include "Iterable.h"
#include <iostream>
using namespace std;


// 构造函数
ASTree::ASTree(const vector<std::shared_ptr<ASTree>>& astrees) {};
ASTree::ASTree() = default;   // 最简单实现

// 返回子节点迭代对象
Iterable<std::shared_ptr<ASTree> > ASTree::iterable() const{
    return children;
}

// 被virtual表示的必须在cpp中实现的成员函数
std::shared_ptr<ASTree> ASTree::child(const int i) const {
    auto it = children.begin();
    std::advance(it, i);
    return *it;
}

int ASTree::numChildren() const {
    int count = 0;
    for (auto it = children.begin(); it != children.end(); ++it) {
        ++count;
    }
    return count;
}


std::string ASTree::toString() {
    return "ASTree";  // 或者你自己的逻辑
}

std::string ASTree::location() const {
    return "unknown";  // 或者返回文件名/行号等
}

