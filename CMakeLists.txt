cmake_minimum_required(VERSION 3.15)

project(
    cpplox 
    	LANGUAGES CXX
)
        
add_executable(
    cpplox
        source/AstPrinter.cpp
        source/AstPrinter.hpp
        source/Common.hpp
        source/Environment.cpp
        source/Environment.hpp
        source/Expr.hpp
        source/Interpreter.cpp
        source/Interpreter.hpp
        source/LoxClass.cpp
        source/LoxClass.hpp
        source/LoxInstance.cpp
        source/LoxInstance.hpp
        source/main.cpp
        source/Parser.cpp
        source/Parser.hpp
        source/ParserError.hpp
        source/Resolver.cpp
        source/Resolver.hpp
        source/RuntimeError.hpp
        source/Scanner.cpp
        source/Scanner.hpp
        source/ScannerError.hpp
        source/Stmt.hpp
        source/Token.hpp
        source/TokenType.hpp
) 

target_compile_features(
     cpplox
         PUBLIC
         cxx_std_23
)
