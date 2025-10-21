//
// Created by xskj on 2025/10/18.
//

#include "Iterable.h"
#include <string>

// 把项目中**真正会用到**的类型提前实例化到当前翻译单元，
// 避免每个 .cpp 都重新实例化一次，加快链接。
template class Iterable<int>;
template class Iterable<double>;
template class Iterable<std::string>;
// 如果还有自定义 AST 类型，继续加：
// template class Iterable<std::shared_ptr<ASTree>>;