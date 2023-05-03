cmake_minimum_required(VERSION 3.22)
project(Skipt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Skipt main.cpp Extensions/StringExt.h Property/Property.h Property/PropertyFile.cpp Property/PropertyFile.h Property/PropertyReference.cpp Property/PropertyReference.h Compiler/Variable.cpp Compiler/Variable.h Compiler/DefaultFiles.h Compiler/FileReader.cpp Compiler/FileReader.h Compiler/Compile.cpp Compiler/Compile.h Compiler/ExpressionParser/Parser.cpp Compiler/ExpressionParser/Parser.h Compiler/ExpressionParser/Tokenizer.cpp Compiler/ExpressionParser/Tokenizer.h Compiler/ExpressionParser/Expression.cpp Compiler/ExpressionParser/Expression.h Compiler/Conditional/Operator.h Compiler/Function/Functions.cpp Compiler/Function/Functions.h Compiler/Function/SystemFunction.cpp Compiler/Function/SystemFunction.h Compiler/Function/StringFunction.cpp Compiler/Function/StringFunction.h Compiler/Function/InternalFunction.cpp Compiler/Function/InternalFunction.h Compiler/Function/MathFunction.cpp Compiler/Function/MathFunction.h Compiler/ExitMessage.h Compiler/Function/ArrayFunctions.cpp Compiler/Function/ArrayFunctions.h ModifiedType/String.cpp ModifiedType/String.h Extensions/Array.cpp Extensions/Array.h Compiler/Function/StatisticsFunction.cpp Compiler/Function/StatisticsFunction.h)
