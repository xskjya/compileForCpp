//
// Created by xskj on 2025/10/18.
//

#include "../../../../../include/WhileStmnt.h"


WhileStmnt::WhileStmnt(vector<std::shared_ptr<ASTree>>& c):ASTList(c){}

ASTree* WhileStmnt::condition() {
    return child(0).get();
}


// 循环体
ASTree* WhileStmnt::body() {
    return child(1).get();
}

// 重载方法
string WhileStmnt::toString()  {
    return "(while " + condition()->toString()
        + body()->toString() + ")";
}