//
// Created by xskj on 2025/10/19.
//

#include "../../include/tokenElementChild.h"
#include "Lexer.h"   // 用到 peek/read 等

namespace tokenElementChild {

    /* ---------- ATokenElement 实现 ---------- */
    template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
    ATokenElement<leafType>::ATokenElement() {
        static_assert(std::is_base_of<ASTLeaf, leafType>::value, "LeafType must derive from ASTLeaf");

	    // 获取对应的工厂对象
	    factory = Factory::get<leafType, Token*>();
    }

    // 重载match函数
    template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
    bool ATokenElement<leafType>::match(Lexer& lexer) const {
        // 取下一个 token（但不消费它），若 null 则返回 false，否则调用 test(t)。
        Token* t = lexer.peek(0);        // 查看下一个token
        if (t == nullptr) return false; // 没有token
        return test(*t); // 测试token类型
    }


    // 解析器
    template<typename leafType>  // 模板参数：叶节点类型，默认为 ASTLeaf
    void ATokenElement<leafType>::parse(Lexer& lexer, std::vector<std::shared_ptr<ASTree>>& res)  {

        // 读取token
        Token* t = lexer.read();

        // 匹配token
        if (test(*t)) {
            // 利用工厂函数创建叶节点ASTLeaf对象
            std::shared_ptr<ASTree> leaf = factory->make(t);

            // 添加进结果res
            res.push_back(leaf);
        }
        else {
            // token类型不匹配，抛异常
            throw ParseException(t->getText() + " type mismatch");
        }
    }



    /* ---------- NumTokenElement 实现 ---------- */
    // ------------------------------------------------------
    // NumTokenElement —— 匹配数字（Number）
    // ------------------------------------------------------
    template<typename leafType>
    NumTokenElement<leafType>::NumTokenElement() : ATokenElement<leafType>() {}

    template<typename leafType>
    bool NumTokenElement<leafType>::test(const Token& t) const  {
            return t.isNumber();
        }

    /* ---------- IdTokenElement 实现 ---------- */
    // ------------------------------------------------------
    // IdTokenElement —— 匹配标识符（Identifier），排除保留字
    // ------------------------------------------------------
    template<typename leafType>
    IdTokenElement<leafType>::IdTokenElement(const std::set<std::string>& r)
        : ATokenElement<leafType>() {
        if (!r.empty()) reserved = r;
    }


    template<typename leafType>
    bool IdTokenElement<leafType>::test(const Token& t) const {
        return t.isIdentifier() &&
            (reserved.find(t.getText()) == reserved.end());
    }


    /* ---------- StrTokenElement 实现 ---------- */
    // ------------------------------------------------------
    // StrTokenElement —— 匹配字符串（String）
    // ------------------------------------------------------
    template<typename leafType>
    StrTokenElement<leafType>::StrTokenElement() : ATokenElement<leafType>() {}

    template<typename leafType>
    bool StrTokenElement<leafType>::test(const Token& t) const  {
            return t.isString();
        }

} // namespace tokenElementChild
