//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_NEGATIVEEXPR_H
#define MYPROJECT_NEGATIVEEXPR_H


#include "ASTList.h"


// 定义了一个 一元负号表达式（NegativeExpr），用于处理 -x 这样的语法结构
// 语法树中代表 “负号表达式” 的节点，比如 -3、-x 等
class NegativeExpr : public ASTList {
public:
    explicit NegativeExpr(const std::vector<std::shared_ptr<ASTree>>& c);

    /* 取得唯一操作数（-x 里的 x） */
    std::shared_ptr<ASTree> operand();

    /* 打印成 "-x" 形式 */
    std::string toString() override;
};

#endif //MYPROJECT_NEGATIVEEXPR_H