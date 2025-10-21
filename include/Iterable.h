//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_ITERABLE_H
#define MYPROJECT_ITERABLE_H


#include <initializer_list>
#include <vector>
using namespace std;


template <typename T>
class Iterable {
public:
    Iterable() = default;

    // 支持 vector 复制或移动
    Iterable(const vector<T>& values) : data(values) {}
    Iterable(vector<T>&& values) : data(std::move(values)) {}

    // ✅ 新增：支持花括号初始化
    Iterable(std::initializer_list<T> values) : data(values) {}

    // 可迭代接口
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

private:
    vector<T> data;
};

#endif //MYPROJECT_ITERABLE_H