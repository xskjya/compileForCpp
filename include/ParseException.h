//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_PARSEEXCEPTION_H
#define MYPROJECT_PARSEEXCEPTION_H

#include <exception>
#include "Token.h"
#include <boost/stacktrace.hpp>

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

    static std::string buildMessage(const std::string& message,
                                    const char* file,
                                    int line) {
        std::string stk = boost::stacktrace::to_string(
            boost::stacktrace::stacktrace());

        return "[ParseException] " + message + "\n"
             + "  at " + file + ":" + std::to_string(line) + "\n"
             + "Stack trace:\n" + stk;
    }
};

// 宏：一行抛出，自动带文件、行号、调用栈
#define THROW_PARSE_EXCEPTION(msg) \
throw ParseException(msg, __FILE__, __LINE__)

#endif //MYPROJECT_PARSEEXCEPTION_H