//
// Created by xskj on 2025/10/19.
//

#ifndef MYPROJECT_TOKENELEMENTCHILD_H
#define MYPROJECT_TOKENELEMENTCHILD_H

#include <string>
#include <set>
#include <vector>
#include <memory>
#include "ASTLeaf.h"
#include "Element.h"
#include "Factory.h"
#include "ParseException.h"

namespace tokenElementChild {

    /* -------------- 前向声明 -------------- */
    template<typename leafType = ASTLeaf>
    class ATokenElement;

    template<typename leafType = ASTLeaf>
    class NumTokenElement;

    template<typename leafType = ASTLeaf>
    class IdTokenElement;

    template<typename leafType = ASTLeaf>
    class StrTokenElement;



    // ATokenElement: 解析一个特定类型的 Token（如标识符、数字、字符串等）；用于匹配具体的 TokenType。
    // 基于 token 的原子解析 抽象类，集中处理“从 Lexer 读一个 token 并根据 test(Token t) 判断是否符合”的模式：
    // 用途：匹配标识符、数字、字符串字面量等原子 token。
    // 是一种 “原子语法元素”（atomic element）——在解析器组合子结构中负责匹配并生成 单个叶子节点（ASTLeaf）
    // ASTLeaf的子类为NumberLiteral和Name，分别用于匹配数字和标识符
    // 默认为 ASTLeaf 节点，但可通过工厂函数定制为其他类型的叶节点（如 NumberLiteral、Name 等）。
    //数字 token → NumberLiteral.class
    //标识符 token → Name.class
    //默认情况 → ASTLeaf.class
    template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
    class ATokenElement : public Element {
    protected:

        // AST 节点工厂
        std::shared_ptr<Factory> factory;

        // =====================
        // 构造函数模板：传入 ASTLeaf 派生类类型
        // 如果未传入类型，则默认使用 ASTLeaf
        // =====================
        // 修复：为 Factory::get 增加默认参数类型（Token），以匹配模板定义
        ATokenElement();

    public:
        // 重载match函数
        bool match(Lexer& lexer) const override ;

        // 解析器
        void parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res)  override;

        // 子类实现
        virtual bool test(const Token& t) const  = 0;
    };


    // ------------------------------------------------------
    // NumTokenElement —— 匹配数字（Number）
    // ------------------------------------------------------
    template<typename leafType>
    class NumTokenElement : public ATokenElement<leafType> {
    public:
        NumTokenElement();

        bool test(const Token& t) const override;
    };


    // ------------------------------------------------------
    // IdTokenElement —— 匹配标识符（Identifier），排除保留字
    // ------------------------------------------------------
    template<typename leafType>
    class IdTokenElement : public ATokenElement<leafType> {
    public:
        static std::set<std::string> reserved; // 静态共享保留字表

        explicit IdTokenElement(const std::set<std::string>& r = {});

        bool test(const Token& t) const override ;
    };


    // ------------------------------------------------------
    // StrTokenElement —— 匹配字符串（String）
    // ------------------------------------------------------
    template<typename leafType>
    class StrTokenElement : public ATokenElement<leafType> {
    public:
        StrTokenElement();

        bool test(const Token& t) const override;
    };
} // namespace tokenElementChild

#endif //MYPROJECT_TOKENELEMENTCHILD_H