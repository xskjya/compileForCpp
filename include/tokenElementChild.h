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



    template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
    class ATokenElement : public Element {
protected:
    std::shared_ptr<Factory> factory;                // AST 节点工厂
    // =====================
    // 构造函数模板：传入 ASTLeaf 派生类类型
    // 如果未传入类型，则默认使用 ASTLeaf
    // =====================
    // 修复：为 Factory::get 增加默认参数类型（Token），以匹配模板定义
    ATokenElement() ;
public:
    // 重载match函数
    bool match(Lexer& lexer) const override;

    // 解析器
    void parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res)  override;

    // 子类实现
    virtual bool test(const Token& t) const = 0;
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