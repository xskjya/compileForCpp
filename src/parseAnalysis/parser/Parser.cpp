//
// Created by xskj on 2025/10/18.
//

#include "../../../include/Parser.h"


// 拷贝构造函数：复制另一个 Parser 的元素与工厂
Parser::Parser(const Parser& p) {
    elements = p.elements;  // 拷贝语法单元序列
    factory = p.factory;    // 拷贝对应工厂
}
Parser::Parser()  = default;                        // 默认构造


// 非模板版本
// std::shared_ptr<Parser> Parser::rule() { return std::make_shared<Parser>(); }

// 析构
Parser::~Parser() = default;


