//
// Created by xskj on 2025/10/18.
//

#include "../../include/StoneException.h"
#include <iostream>
#include "ASTree.h"
#include <string>
using namespace std;



// 进一个参数的构造函数
StoneException::StoneException(const string& m):runtime_error(m) {}


// 有两个参数的构造函数
StoneException::StoneException(const string& m, const ASTree& t):runtime_error(m + "   " + t.location()){}