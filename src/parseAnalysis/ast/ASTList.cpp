//
// Created by xskj on 2025/10/18.
//

#include "../../../include/ASTList.h"


ASTList::ASTList(const vector<std::shared_ptr<ASTree>> astrees): ASTree(astrees), children(astrees){};

//virtual允许子类重写该方法，和接口类的设计理念一致
// 查询第i个节点(ASTree对象: ASTLeaf或ASTList
std::shared_ptr<ASTree> ASTList::child(const int i) const {
    return children[i];
};

// 返回子节点(即ASTree对象: ASTLeaf或ASTList)的个数(无则返回0
int ASTList::numChildren() const {
    return  static_cast<int>(children.size());
};

// 获取子节点的迭代对象
Iterable<std::shared_ptr<ASTree>> ASTList::childrenIterable() const {
    return iterable();
}

// 转化为字符串
string ASTList::toString()   {

    std::ostringstream builder;
    builder << "[";

    for (std::shared_ptr<ASTree>  p : children) {
        builder << p->toString() << ",";
    }

    builder << "]";

    return builder.str();
}


// 定位函数
std::string ASTList::location() const {
    for (auto& t : children) {
        std::string s = t->location();
        if (!s.empty())
            return s;
    }
    return "";
}

