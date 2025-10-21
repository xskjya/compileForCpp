//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_LEXER_H
#define MYPROJECT_LEXER_H

#include <string>
#include <vector>
#include <deque>
#include "Reader.h"
#include "Token.h"
#include "boost/regex/v5/basic_regex.hpp"
#include "boost/regex/v5/regex_fwd.hpp"
using namespace std;


class Lexer {
public:
    Lexer(Reader& r);
    Lexer();
    ~Lexer();

    // 初始化正则表达式
    void regexInitial();

    // 源代码Reader对象
    Reader* reader;

    // 从队列开头读取Token对象并返回
    Token* read();

    // 预读第i个Token对象但不移除
    Token* peek( int i);

    // 判断队列中token是否为空
    bool fillQueue(int i);

    // 表示还有更多行可读
    bool hasMore;

    // 读取一行源码，转化为tokens后加入queue
    void readLine();

    // 源代码Token化
    void tokenize(const string& code, const int l);

    // 增加Token
    void addToken(TokenType type, int lineNo, const string& text);

    // 获取Tokens大小
    size_t getTokensSize() const;

    // 对Line对象实现Token化
    void lineTokenize(const Line& line);

private:
    vector<string> tokens;
    boost::regex tokenRegex;
    string regexPat;

    // 处理字符串中的转义字符
    string toStringLiteral(const string s) const;

protected:
    // 用于存储Token的队列
    deque<Token*> queue;

};

#endif //MYPROJECT_LEXER_H