//
// Created by xskj on 2025/10/18.
//

#include "../../include/Line.h"

// 默认构造：生成一个“无效行”对象
Line::Line() : lineNumber(0), content("") {}

// 带参构造：外部传入真实行号与源码内容
Line::Line(int l, const std::string& c) : lineNumber(l), content(c) {}

// 布尔转换：行号>0 视为有效
Line::operator bool() const {
    return lineNumber > 0;
}

// 返回行号
int Line::getLineNumber() const {
    return lineNumber;
}

// 返回行内容常量引用，避免拷贝
const std::string& Line::getContent() const {
    return content;
}