//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_LINE_H
#define MYPROJECT_LINE_H
#include <string>

/**
 * @brief 源代码行号与内容封装
 *        用于词法/语法分析阶段记录源码位置信息。
 */
class Line {
public:
    Line();                           ///< 默认构造：行号=0，内容空
    explicit Line(int l, const std::string& c); ///< 带行号与内容构造

    /**
     * @brief 布尔转换运算符
     * @return true  表示有效行（行号>0）
     * @return false 表示无效/默认行
     */
    explicit operator bool() const;

    int  getLineNumber() const;           ///< 获取行号
    const std::string& getContent() const;///< 获取行内容

private:
    int         lineNumber; ///< 1-based 行号，0 代表无效
    std::string content;    ///< 该行源码文本（不含换行符）
};

#endif //MYPROJECT_LINE_H