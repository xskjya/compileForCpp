//
// Created by xskj on 2025/10/26.
//

#ifndef MYPROJECT_UTILS_H
#define MYPROJECT_UTILS_H

#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <vector>


#if defined(__GNUG__)
#include <cxxabi.h>


// ✅ GNU/Clang 平台（Linux, macOS）
inline std::string demangle(const char* name) {
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res{
        abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free
    };
    return (status == 0 && res) ? res.get() : name;
}
#elif defined(_MSC_VER)
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

// ✅ MSVC 平台（Windows）
inline std::string demangle(const char* name) {
    char demangled[1024];
    if (UnDecorateSymbolName(name, demangled, sizeof(demangled), UNDNAME_COMPLETE))
        return demangled;
    return name;
}
#else
// ✅ 其他编译器（安全兜底）
inline std::string demangle(const char* name) { return name; }
#endif


// 🧩 输出模板类型信息（例如 Factory::get<T, ArgType>）
template<typename T, typename ArgType>
inline void printTypeInfo(const std::string& prefix = "[TypeInfo]") {
    std::cout << prefix << " T        = " << demangle(typeid(T).name()) << std::endl;
    std::cout << prefix << " ArgType  = " << demangle(typeid(ArgType).name()) << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
}

// 🧩 输出对象的动态类型信息（多态对象自动识别）
template<typename ObjType>
inline void printTypeInfo(const ObjType& obj, const std::string& prefix = "[ObjectType]") {
    std::cout << prefix << " = " << demangle(typeid(obj).name()) << std::endl;
}


// *********************封装BNF打印函数********************************
// 打印调试Or语法： 对应 |
template <typename P>
void printfOr(const std::vector<std::shared_ptr<P>>& parsers) {
    for (std::size_t i = 0; i < parsers.size(); ++i) {
        std::cout << demangle(typeid(*parsers[i]).name());
        if (i + 1 != parsers.size()) std::cout << " | ";
    }
    std::cout << std::endl;
}



#endif //MYPROJECT_UTILS_H