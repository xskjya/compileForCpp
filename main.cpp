#include <iostream>

#include "include/BasicParser.h"
#include "include/Iterable.h"
#include "include/Lexer.h"
#include "include/Reader.h"
using namespace std;



void testLexer();
void testQueue();
void testReader(const string& filename);
void testIterable();
void testLexerWithPeek();
void testParser();
void test(int& i);

int main() {
    //testIterable();

    // 测试Lexer词法分析器
    //testLexer();
	//  测试Lexer词法分析器的预读功能,预加载
    // testLexerWithPeek();

    // 测试队列数据结构
    //testQueue();

    // 测试从源文件中逐行读取源代码
    //testReader("test.cpp");  // 测试文件名


    // 测试迭代
    //testIterable();

    // 测试语法解析器
    testParser();

    return 0;
}


void test(int& t) {
    t= 10;
}


// 测试语法分析器
void testParser() {
    // 获取词法分析器对象
    std::string filename = "../assets/test.cpp";
    Reader reader(filename);
    Lexer lexer(reader);  // 内部会进行自动token化

    // 打印
    cout << "token size: " << lexer.getTokensSize() << endl;

    // 实例化一个基础解析器对象
    BasicParser bp = BasicParser();

    // 对Lexer词法分析器中的token进行循环遍历进行语法分析处理，生成AST
    while (lexer.peek(0)->getText() != Token::EoF) {  // 目的是保证整个循环执行，直到token为EOF结束

        /*
         * 调用实现预定义的BNF语法基础解析器的解析接口进行处理
         * 操作对象： 词法分析器对象lexer
         * 接口方法： parse(Lexer& lexer)
         */
        std::shared_ptr<ASTree>  ast = bp.parse(lexer);

        /*
         * 打印出抽象语法树:
         * ASTList类的toString方法将调用所有子节点的ASTree对象的toString方法，
         * 并用空白符连接所有字符串，最后在两侧添加括号后返回
         */
        cout << "==> " << ast->toString() << endl;
    }
}


void testIterable() {
    Iterable<int> c = { 1, 2, 3, 4 };

    for (int x : c) {
        cout << x << endl;
    }

}


// 测试从文件中逐行读取源代码
void testReader(const string& filename) {
    try {
        Reader reader(filename);
        while (true) {
            Line line = reader.readLine();
            cout << "Line " << line.getLineNumber() << ": " << line.getContent() << endl;
        }
    }
    catch (const StoneException& e) {
        cout << "Reader finished or error: " << e.what() << endl;
    }
}


void testLexerWithPeek() {
    std::string filename = "../assets/test.cpp";
    Reader reader(filename);
    Lexer lexer(reader);


    // 预读下一个 token（未移除）
    int i = 1;
    Token* nextToken = lexer.peek(i);

    cout << "[peek] " << i << " token: " << nextToken->getText()
        << " (line " << nextToken->getLineNo() << ")" << std::endl;


    // 获取并移除下一个 token
    Token* token;
	cout << "Reading tokens until EOF:" << std::endl;
    while ((token = lexer.read())->getLineNo() != -1) {
        cout << "[read] token: " << token->getText()
            << " (line " << token->getLineNo() << ")" << std::endl;
    }

}



// 测试Lexer词法分析器
void testLexer() {

    //方式一: token化
    //lexer.tokenize(code, 1);

    //方式二： 行原代码token化
    //Line line(1, code);
    //lexer.lineTokenize(line);

    //方式三: 使用Reader
    //string filename = "test.cpp";
    //Reader reader(filename);
    //Line line =  reader.readLine();
    //lexer.lineTokenize(line);

    // 方式四：循环编列每一行原代码进行lexer分析
    //Lexer lexer;
    //string filename = "test.cpp";
    //Reader reader(filename);
    //Line line;
    //while ((line = reader.readLine()).getLineNumber() != -1)
    //    // line token化
    //    lexer.lineTokenize(line);

    //std::cout << "总 token 数: " << lexer.getTokensSize() << std::endl;

    //// 方式五：采用按需加载读取源代码
    string filename = "./assets/test.cpp";
    Reader reader(filename);
    Lexer lexer(reader);

	int count = 0;
    while (true)
    {
        Token* token = lexer.read();
        if(token->getLineNo() == -1) // 文件结束符
			break;
        count++;
    }
    std::cout << "总 token 数: " << count << std::endl;

}



// // 测试Parser语法分析器
// void testParser() {
//     // lexer分析
//     Lexer lexer;
//     string filename = "test.cpp";
//     Reader reader(filename);
//     Line line;
//
// 	// 标识文件结束符
// 	signed int EoF = -1;
//     while ((line = reader.readLine()).getLineNumber() != EoF)
//         // line token化
//         lexer.lineTokenize(line);
//
// 	// 语法分析
//
// }
