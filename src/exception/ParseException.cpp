//
// Created by xskj on 2025/10/18.
//

#include "../../include/ParseException.h"
#include "Token.h"
#include <iostream>
using namespace std;





ParseException::ParseException(const std::string& m, const Token& t)
    : msg("syntax error around " + location(t) + ". " + m) {}

ParseException::ParseException(const std::string& m)
    : msg("syntax error around: " + m) {}

std::string ParseException::location(const Token& t) const {
    return "line " + t.getLineNo();   // 按需调整格式
}

// 重载what
const char* ParseException::what() const noexcept {
    return msg.c_str();
}
