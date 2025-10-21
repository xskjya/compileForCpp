//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_PARSEEXCEPTION_H
#define MYPROJECT_PARSEEXCEPTION_H

#include <exception>
#include "Token.h"

class Token;

class ParseException : public std::exception {
public:
    ParseException(const std::string& m, const Token& t);
    explicit ParseException(const std::string& m);

    /* 根据 Token 生成“行号”提示串 */
    std::string location(const Token& t) const;

    const char* what() const noexcept override;

private:
    std::string msg;
};


#endif //MYPROJECT_PARSEEXCEPTION_H