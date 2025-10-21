//
// Created by xskj on 2025/10/18.
//

#include "../../include/Lexer.h"
#include <iostream>
#include "Token.h"
#include "../../include/Line.h"
#include "Reader.h"
#include "StoneException.h"
#include <boost/regex.hpp>
#include <deque>


// 带Reader参数的构造函数，初始化正则表达式
Lexer::Lexer(Reader& r) : reader(&r), hasMore(true) {
    // 初始化正则表达式
    regexInitial();
}


// 默认构造函数，初始化正则表达式
Lexer::Lexer() : hasMore(true) {
	// 初始化正则表达式
    regexInitial();
}

// 析构函数，释放内存
Lexer::~Lexer() {
    while (!queue.empty()) {
        delete queue.front();
        queue.pop_front();
    }

}


// regexPat
void Lexer::regexInitial() {
    // 用 raw string literal 写正则，便于阅读和维护
    // 捕获组顺序：
    // (1) 注释
    // (2) 数字
    // (3) 字符串（包含引号，内部使用非捕获组避免增加编号）
    // (4) 标识符
    // (5) 复合运算符（多字符）
    // (6) 单字符标点或单字符运算符
    regexPat =
        "(//[^\\n]*)"                                 // (1) 注释
        "|([0-9]+)"                                   // (2) 数字
        "|(\"(?:\\\\.|[^\"\\\\])*\")"                 // (3) 字符串
        "|([A-Za-z_][A-Za-z0-9_]*)"                   // (4) 标识符
        "|(==|<=|>=|&&|\\|\\||\\+\\+|--|::|->|<<|>>)" // (5) 复合运算符
        "|([{}()\\[\\];,.\\+\\-*/%&|^~!=<>?:])";      // (6) 单字符标点/运算符


    // 用 perl 模式，以便支持 (?: ) 等
    tokenRegex = boost::regex(regexPat, boost::regex::perl);
}




// 源代码token化实现
void Lexer::tokenize(const std::string& code, const int l) {
    tokens.clear();

    boost::sregex_iterator it(code.begin(), code.end(), tokenRegex);
    boost::sregex_iterator end;

    // 用于调试：打印每个 match 的所有捕获组（0..6）
    for (; it != end; ++it) {

        boost::smatch m = *it;
        string whole = m.str(0); // 整个匹配

        // 调试输出：查看每个捕获组是否 matched（你可以注释掉）
        // for (size_t i = 0; i < m.size(); ++i) {
        //     if (m[i].matched) {
        //         cout << " grp[" << i << "] = \"" << m[i].str() << "\"";
        //     }
        // }
        // cout << endl;

        TokenType type;
        string text;

        if (m[1].matched) {
            // 注释，忽略
            continue;
        }
        else if (m[2].matched) {
            // 数字
            type = TokenType::numberToken;
            text = m[2].str();

            //cout << "数字: " << text << endl;
        }
        else if (m[3].matched) {
            // 字符串
            string s = m[3].str();               // 包含两侧引号
            string inner = s.substr(1, s.size() - 2); // 去掉引号


            type = TokenType::strToken;
            text = inner;

            //cout << "字符串: " << inner << endl;
        }
        else if (m[4].matched) {
            // 标识符
            type = TokenType::idToken;
            text = m[4].str();

            //cout << "标识符: " << text << endl;
        }
        else if (m[5].matched) {
            // 字复合运算符
            type = TokenType::symbolToken;
            text = m[5].str();

            //cout << "运算符和标点: " << text << endl;
        }
        else if (m[6].matched) {
            // 单字符标点/运算符
            type = TokenType::symbolToken;
            text = m[6].str();

            //cout << "运算符和标点: " << text << endl;
        }
        else {
            // 未知
            //cout << "未知: " << whole << endl;

            // 跳过
            continue;
        }

        // 添加进队列中
        addToken(type, l, text);
    }
}


// 增加Token
void Lexer::addToken(TokenType type, int lineNo, const string& text) {

    // 基类指针
    Token* token = nullptr;

    // 判断
    switch (type)
    {

    case TokenType::eofToken:
        token = new EoFToken();
        break;

    case TokenType::eolToken:
        token = new EoLToken(lineNo);
        break;

    case TokenType::idToken:
        token = new IdToken(lineNo, text);
        break;
    case TokenType::numberToken:
        token = new NumToken(lineNo, text);
        break;

    case TokenType::strToken:
        token = new StrToken(lineNo, text);
        break;
    case TokenType::symbolToken:
        token = new SymbolToken(lineNo, text);
        break;
    default:
        throw StoneException("未知Token类型!");
    }

    // 添加进入token队列中
    queue.push_back(token);
}

// 获取token大小
size_t Lexer::getTokensSize() const{
    return queue.size();
}



// 对Line对象实现Token化
void Lexer::lineTokenize(const Line& line) {
    int l = line.getLineNumber();
    string text = line.getContent();
    tokenize(text, l);
}


// 处理字符串中的转义字符
string Lexer::toStringLiteral(const string s) const {
    return "";
}




// 读取一行源码并 token 化
void Lexer::readLine() {
    Line line = reader->readLine();

    if (line.getLineNumber() == -1) {
        hasMore = false;
        addToken(TokenType::eofToken, -1, "EOF"); // EOF token
        return;
    }

    lineTokenize(line);
}

// fillQueue：确保队列中至少有 i+1 个 token
bool Lexer::fillQueue(int i) {
    while (i >= static_cast<int>(queue.size())) { // 转换 size_t -> int
        if (hasMore) {
			readLine();// 读取并 token 化一行
        }
        else {
            return false; // 没有更多行可读
        }
    }
    return true;
}



// 从队列开头读取并移除
Token* Lexer::read() {
    if (fillQueue(0)) {
        Token* t = queue.front();
        queue.pop_front();  // deque 高效删除队头
        return t;
    }
    else {
        throw StoneException("没有更多Token可读!");
    }
}


// 预读第 i 个 token，不移除
Token* Lexer::peek(int i) {
    if (fillQueue(i)) {
        return queue[i];
    }
    else {
        throw StoneException("没有足够的Token可读!");
    }
}




