//
// Created by xskj on 2025/10/18.
//
#include "ASTree.h"
#include "ASTList.h"
#include "../../include/Factory.h"


// /* 非模板实现 */
// std::shared_ptr<ASTree> Factory::make(void* arg) {
//     try {
//         return make0(arg);
//     } catch (const std::invalid_argument&) {
//         throw;
//     } catch (const std::exception& e) {
//         throw std::runtime_error(std::string("Factory::make failed: ") + e.what());
//     }
// }
//
// /* ---------- get<T, ArgType> 实现 ---------- */
// template <typename T, typename ArgType>
// std::shared_ptr<Factory> Factory::get() {
//     if constexpr (std::is_void_v<T>)
//         return nullptr;
//
//     /* 情形 1：存在静态 T::create(ArgType) */
//     if constexpr (requires(ArgType a) { T::create(a); }) {
//         struct StaticCreateFactory : Factory {
//             std::shared_ptr<ASTree> make0(void* arg) override {
//                 auto* realArg = static_cast<ArgType*>(arg);
//                 return T::create(*realArg);
//             }
//         };
//         return std::make_shared<StaticCreateFactory>();
//     }
//     /* 情形 2：存在构造函数 T(ArgType) */
//     else if constexpr (std::is_constructible_v<T, ArgType>) {
//         struct ConstructorFactory : Factory {
//             std::shared_ptr<ASTree> make0(void* arg) override {
//                 auto* realArg = static_cast<ArgType*>(arg);
//                 return std::make_shared<T>(*realArg);
//             }
//         };
//         return std::make_shared<ConstructorFactory>();
//     }
//     else {
//         throw std::runtime_error(
//             "Factory::get - no suitable create() or constructor found for type");
//     }
// }
//
// /* ---------- getForASTList<T> 实现 ---------- */
// template <typename T>
// std::shared_ptr<Factory> Factory::getForASTList() {
//     /* 优先尝试用户特化工厂 */
//     if (auto f = get<T, std::vector<std::shared_ptr<ASTree>>>())
//         return f;
//
//     /* 默认工厂：单节点直接返回，多节点包成 ASTList */
//     struct DefaultFactory : Factory {
//         std::shared_ptr<ASTree> make0(void* arg) override {
//             auto* results = static_cast<std::vector<std::shared_ptr<ASTree>>*>(arg);
//             if (results->size() == 1) return (*results)[0];
//             return std::make_shared<ASTList>(*results);
//         }
//     };
//     return std::make_shared<DefaultFactory>();
// }
//
// /* ======== 显式实例化常用模板 ======== */
// template std::shared_ptr<Factory> Factory::get<ASTree , std::vector<std::shared_ptr<ASTree>>>();
// template std::shared_ptr<Factory> Factory::get<ASTList, std::vector<std::shared_ptr<ASTree>>>();
// template std::shared_ptr<Factory> Factory::getForASTList<ASTree>();
// template std::shared_ptr<Factory> Factory::getForASTList<ASTList>();