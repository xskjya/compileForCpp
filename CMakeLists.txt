# 最低版本
cmake_minimum_required(VERSION 3.16)  # CMake 4.0 不存在，最新版是 3.x

# 项目名称
project(MyProject CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 源文件收集
aux_source_directory(src SRC)

# 头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/boost_1_89_0)

# 可执行文件
add_executable(mainExecute ${SRC} main.cpp
        src/common/Factory.cpp
        include/Factory.h
        src/common/Iterable.cpp
        include/Iterable.h
        src/exception/ParseException.cpp
        include/ParseException.h
        src/exception/StoneException.cpp
        include/StoneException.h
        src/lexerAnalysis/Lexer.cpp
        include/Lexer.h
        src/lexerAnalysis/Line.cpp
        include/Line.h
        src/lexerAnalysis/Reader.cpp
        include/Reader.h
        src/lexerAnalysis/Token.cpp
        include/Token.h
        src/parseAnalysis/ast/leafRules/name/Name.cpp
        include/Name.h
        src/parseAnalysis/ast/leafRules/value/NumberLiteral.cpp
        include/NumberLiteral.h
        src/parseAnalysis/ast/leafRules/value/StringLiteral.cpp
        include/StringLiteral.h
        src/parseAnalysis/ast/listRules/expr/BinaryExpr.cpp
        include/BinaryExpr.h
        src/parseAnalysis/ast/listRules/expr/NegativeExpr.cpp
        include/NegativeExpr.h
        src/parseAnalysis/ast/listRules/expr/PrimaryExpr.cpp
        include/PrimaryExpr.h
        src/parseAnalysis/ast/listRules/stmnt/BlockStmnt.cpp
        include/BlockStmnt.h
        src/parseAnalysis/ast/listRules/stmnt/IfStmnt.cpp
        include/IfStmnt.h
        src/parseAnalysis/ast/listRules/stmnt/NUllStmnt.cpp
        include/NUllStmnt.h
        src/parseAnalysis/ast/listRules/stmnt/WhileStmnt.cpp
        include/WhileStmnt.h
        src/parseAnalysis/ast/ASTLeaf.cpp
        include/ASTLeaf.h
        src/parseAnalysis/ast/ASTList.cpp
        include/ASTList.h
        src/parseAnalysis/ast/ASTree.cpp
        include/ASTree.h
        src/parseAnalysis/parser/Element.cpp
        include/Element.h
        src/parseAnalysis/parser/Parser.cpp
        include/Parser.h
        src/common/elementChild.cpp
        include/elementChild.h
        src/common/tokenElementChild.cpp
        include/tokenElementChild.h
        src/parseAnalysis/parser/BasicParser.cpp
        include/BasicParser.h
        src/parseAnalysis/parser/ParserRunner.cpp
        include/ParserRunner.h
)

# Boost 库路径（链接目录）
link_directories(${PROJECT_SOURCE_DIR}/lib/boost_1_89_0/stage/lib)

# 链接 Boost Regex 库
target_link_libraries(mainExecute PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/boost_1_89_0/stage/lib/libboost_regex-mgw15-mt-d-x64-1_89.a
)
