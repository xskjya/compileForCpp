//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_ELEMENT_H
#define MYPROJECT_ELEMENT_H

#include "Lexer.h"

// 语法元素抽象基类 职责：定义两个抽象操作
/*
语义：解析流程中先用 match 决定是否可以用这个 element，然后在 parse 中真正消费 token 并把解析结果放入 res。
*/
class Element {
public:
    // 需要设置成虚函数，以确保派生类的析构函数被调用
    virtual ~Element() = default;

    // 解析函数: 消费 token 并将结果节点 push 到 res
    // 消费输入并把解析出来的 AST 节点 push 到 res 中（产生副作用）。
    virtual void parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res) = 0;

    // 匹配：预测，不消费；若无法预测可能抛出 ParseException
    // 预测（lookahead），不消费，返回当前是否能由该 element 成功开始解析。
    virtual bool match(Lexer& lexer) const = 0;
};



#endif //MYPROJECT_ELEMENT_H