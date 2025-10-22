//
// Created by xskj on 2025/10/18.
//

#include "Token.h"
#include <stdexcept>
using namespace std;






// 获取行号
int Token::getLineNo() const {   // const成员函数，不能修改成员变量  除非变量为mutable修饰
    //lineNumber = 100; //测试const成员函数不能修改成员变量   加const后编译报错
    return lineNumber;
}

// 获取文本
string Token::getText() const {
    return text;
}


signed int Token::getNumber() const {
    throw runtime_error("not number token");
}
