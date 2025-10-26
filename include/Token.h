//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_TOKEN_H
#define MYPROJECT_TOKEN_H


#include <string>
using namespace std;


// Token类型枚举
enum class TokenType {
    eolToken,
    eofToken,
    numberToken,
    idToken,
    strToken,
    symbolToken   // 统一存储运算符和标点
};


class EoFToken;

class Token {

public:
    // 初始化函数
    explicit Token(TokenType tt, int l, const string& t ) : tokenType(tt), lineNumber(l), text(t){}
    Token() : tokenType(TokenType::idToken), lineNumber(0) {}  // 默认构造
    virtual ~Token() = default;

    // 所在行号
    virtual int getLineNo() const;

    // 获取Token字符文本
    virtual string  getText() const;

    // 获取数字值
    virtual signed int getNumber() const;


    // 判断Token类型
    virtual bool isIdentifier() const{ return false;  }
    virtual bool isNumber()     const { return false; }
    virtual bool isString()     const { return false; }

    // ② 现在可以安全实例化
    inline static const std::string EoF = "EOF"; // 默认构造
    // ② 现在可以安全实例化
    inline static const std::string EoL = "EOL"; // 默认构造

protected:
    // 声明行号变量
    signed  int lineNumber; //行号

    // 字符
    string text;

    // Token类型
    TokenType tokenType;
};




//*********************子类Token*********************

// 数字Token
class NumToken : public Token {
public:
    // 初始化
    explicit NumToken(const int l, const string& num) : Token(TokenType::numberToken, l, num){};
};


// 标识符Token
class IdToken : public Token {
public:
    explicit IdToken(const int l, const string& name) : Token(TokenType::idToken, l, name) {};

};

// 字符Token
class StrToken : public Token {
public:
    StrToken(const int l, const string& t) : Token(TokenType::strToken, l, t){};

};

// EOF结束符Toke
class EoFToken : public Token {
public:
    EoFToken(): Token(TokenType::eofToken, -1,Token::EoF) {}
};



// 行结束标识符
class EoLToken : public Token {
public:
    EoLToken(const int l) : Token(TokenType::eolToken,  l ,"\n"){};
};


class SymbolToken : public Token {
public:
    explicit SymbolToken(const int l, const string& t): Token(TokenType::symbolToken, l, t){}
};

#endif //MYPROJECT_TOKEN_H