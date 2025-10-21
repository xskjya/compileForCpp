//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_FACTORY_H
#define MYPROJECT_FACTORY_H

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>

// ① 前置声明（减少其余文件依赖）
class ASTree;

// ② 立即放完整定义，确保模板实例化时类型完整
#include "ASTree.h"   // ASTree 完整定义
#include "ASTList.h"  // ASTList 继承 ASTree 的完整定义
#include "ASTLeaf.h"  // ASTLeaf 完整定义
#include "Token.h"    // Token 完整定义


/**
 * Factory 类
 * ------------------------------
 * 作用：
 *   - 用于在语法分析器(Parser)中创建 AST 节点对象（ASTLeaf、ASTList、或自定义语法树节点）。
 *   - 模拟 Java 版本中的反射机制 (Class.newInstance / getMethod("create"))。
 *   - C++ 没有运行时反射，因此用模板 + 静态检查(requires, std::is_constructible_v) 模拟。
 */
/**
 * Factory
 * ------
 * 创建 AST 节点的统一入口（模拟 Java 反射工厂）
 */


class Factory {
public:
    static constexpr const char* factoryName = "create";
    virtual ~Factory() = default;

    /* 统一对外接口 */
    std::shared_ptr<ASTree> make(void* arg);

    /* 静态工厂：获取 ASTList 专用工厂 */
    template <typename T = ASTree>
    static std::shared_ptr<Factory> getForASTList();

    /* 静态工厂：获取任意节点工厂 */
    template <typename T, typename ArgType>
    static std::shared_ptr<Factory> get();

protected:
    /* 子类真正实现 */
    virtual std::shared_ptr<ASTree> make0(void* arg) = 0;
};

/* ----------------------------------------------------------
 * 非模板实现
 * ---------------------------------------------------------- */
inline std::shared_ptr<ASTree> Factory::make(void* arg) {
    try {
        return make0(arg);
    } catch (const std::invalid_argument&) {
        throw;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Factory::make failed: ") + e.what());
    }
}

/* ----------------------------------------------------------
 * 模板实现
 * ---------------------------------------------------------- */
template <typename T, typename ArgType>
std::shared_ptr<Factory> Factory::get() {
    if constexpr (std::is_void_v<T>)
        return nullptr;

    /* 1. 优先使用静态 T::create(ArgType) */
    if constexpr (requires(ArgType a) { T::create(a); }) {
        struct StaticCreateFactory : Factory {
            std::shared_ptr<ASTree> make0(void* arg) override {
                auto* realArg = static_cast<ArgType*>(arg);
                return T::create(*realArg);
            }
        };
        return std::make_shared<StaticCreateFactory>();
    }
    /* 2. 否则使用构造函数 T(ArgType) */
    else if constexpr (std::is_constructible_v<T, ArgType>) {
        struct ConstructorFactory : Factory {
            std::shared_ptr<ASTree> make0(void* arg) override {
                auto* realArg = static_cast<ArgType*>(arg);
                return std::make_shared<T>(*realArg);
            }
        };
        return std::make_shared<ConstructorFactory>();
    }
    else {
        throw std::runtime_error(
            "Factory::get - no suitable create() or constructor found for type");
    }
}

template <typename T>
std::shared_ptr<Factory> Factory::getForASTList() {
    if (auto f = get<T, std::vector<std::shared_ptr<ASTree>>>())
        return f;

    struct DefaultFactory : Factory {
        std::shared_ptr<ASTree> make0(void* arg) override {
            auto* results = static_cast<std::vector<std::shared_ptr<ASTree>>*>(arg);
            if (results->size() == 1) return (*results)[0];
            return std::make_shared<ASTList>(*results);
        }
    };
    return std::make_shared<DefaultFactory>();
}

/* 显式实例化：所有链接器会用到的组合 */
template std::shared_ptr<Factory> Factory::get<ASTree , std::vector<std::shared_ptr<ASTree>>>();
template std::shared_ptr<Factory> Factory::get<ASTList, std::vector<std::shared_ptr<ASTree>>>();
template std::shared_ptr<Factory> Factory::getForASTList<ASTree>();
template std::shared_ptr<Factory> Factory::getForASTList<ASTList>();

/* ↓↓↓ 这一行就是 clangd 之前找不到的符号 ↓↓↓ */
template std::shared_ptr<Factory> Factory::get<ASTLeaf, Token>();

#endif //MYPROJECT_FACTORY_H