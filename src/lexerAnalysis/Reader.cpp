//
// Created by xskj on 2025/10/18.
//

// Reader.cpp
#include "../../include/Reader.h"

Reader::Reader(const string& filename) : filename(filename), lineNumber(0) {
    f.open(filename, ios::in);
    if (!f.is_open()) {
        throw StoneException("file:" + filename + " open error!");
    }
}

Reader::~Reader() {
    if (f.is_open()) f.close();
}

Line Reader::readLine() {
    if (!getline(f, lineContent)) {
        // 读取文件内容完成EOF
        lineNumber = -1;
        return Line(lineNumber, "");
    }
    return Line(++lineNumber, lineContent);
}