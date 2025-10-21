//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_IFSTMNT_H
#define MYPROJECT_IFSTMNT_H

#include "ASTList.h"


class IfStmnt : public ASTList {
public:
    explicit IfStmnt(std::vector<std::shared_ptr<ASTree>>& c);

    ASTree* condation();
    ASTree* thenBlock();
    ASTree* elseBlock();

    std::string toString() override;
};
#endif //MYPROJECT_IFSTMNT_H