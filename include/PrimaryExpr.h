//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_PRIMARYEXPR_H
#define MYPROJECT_PRIMARYEXPR_H


#include "ASTList.h"


/* 它用于表示最基础的“主表达式（Primary Expression）”，例如字面量、变量、括号表达式等。
 * 与Expr类的区别： Expr类是继承Element的用于表达语法结构中语素的类，而PrimaryExpr是用于语义化表达一类继承ASTList的主表达式（字面量，变量名和括号表达式)
 * 例如：a = b  a = "a"   a = 100  a = (1+3) 中右值[b, "a", 100, (1+3)]的ASTList抽象语法树的语义化表达实现, 只有一个元素时直接用它本身，避免冗余节点，既省内存又方便后续遍历
 * 语法树里的“最小表达式”包装盒，但工厂函数 create 会自动拆包——只有一个元素时直接用它本身，避免冗余节点，既省内存又方便后续遍历
 * 只是一种语义标签，告诉后续遍历者：“这坨列表是主表达式”
*    字面量：123, "hello"
     变量名：x, foo
     括号表达式：(a + b)
 */
class PrimaryExpr: public ASTList
{
public:
    PrimaryExpr(const vector<std::shared_ptr<ASTree>>& list);

    // 工厂函数：当只有一个子节点时直接返回该节点，否则返回一个 PrimaryExpr
    static std::shared_ptr<ASTree> create(std::vector<std::shared_ptr<ASTree>>& c);
};



#endif //MYPROJECT_PRIMARYEXPR_H