cmake_minimum_required(VERSION 3.22)
project(Skipt)

set(CMAKE_CXX_STANDARD 14)

add_executable(Skipt main.cpp Extensions/Array.h Extensions/String.h Property/Property.h Property/PropertyFile.cpp Property/PropertyFile.h Property/PropertyReference.cpp Property/PropertyReference.h Compiler/Token.cpp Compiler/Token.h Compiler/DefaultFiles.h Compiler/FileReader.cpp Compiler/FileReader.h Compiler/Compile.cpp Compiler/Compile.h Compiler/ExpressionParser.cpp Compiler/ExpressionParser.h Compiler/ExpressionParser/Parser.cpp Compiler/ExpressionParser/Parser.h Compiler/ExpressionParser/Tokenizer.cpp Compiler/ExpressionParser/Tokenizer.h Compiler/ExpressionParser/Expression.cpp Compiler/ExpressionParser/Expression.h Compiler/Conditional/Operator.h)
