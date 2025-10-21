//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_STONEEXCEPTION_H
#define MYPROJECT_STONEEXCEPTION_H


#include "ASTree.h"
#include <stdexcept>
using namespace std;

class StoneException : public runtime_error {
public:
    // 进一个参数的构造函数
    StoneException(const string& m);

    // 有两个参数的构造函数
    StoneException(const string& m, const ASTree& t);
};

#endif //MYPROJECT_STONEEXCEPTION_H