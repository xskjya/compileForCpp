//
// Created by xskj on 2025/10/18.
//

#ifndef MYPROJECT_READER_H
#define MYPROJECT_READER_H


#include "Line.h"
#include <fstream>
#include "StoneException.h"
#include <string>
using namespace std;

class Reader {
public:
    explicit Reader(const string& filename);
    ~Reader();

    Line readLine();  // 读取一行

private:
    string filename;
    ifstream f;
    string lineContent;
    int lineNumber;
};


#endif //MYPROJECT_READER_H