// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "Scanner.hpp"

#include "AstPrinter.hpp"
#include "Expr.hpp"
#include "Interpreter.hpp"
#include "Parser.hpp"
#include "Resolver.hpp"
#include "Stmt.hpp"
#include "TokenType.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <variant>
#include <vector>

cpplox::Interpreter interpreter;

void run(const std::string& source) {
    try {
        cpplox::Scanner scanner = cpplox::Scanner(source);
        auto tokens = scanner.scan_tokens();
        auto stmts = cpplox::Parser(tokens).parse();
        
        auto resolver = cpplox::Resolver{interpreter};
        resolver.resolve(stmts);
        
        interpreter.interpret(stmts);
    } catch (const std::exception& exc) {
        std::print("Caught exception: {}\n", exc.what());
    }
    
}

void run_file(const std::string& path) {
    std::ifstream stream;
    stream.open(path);
    
    std::string script;
    std::string str;
    while (std::getline(stream, str)) {
        // std::getline drops the \n, but we need it.
        script += str + "\n";
    }
    
    run(script);
}

void run_prompt() {
    bool stop = false;
    while (!stop) {
        std::string line;
        std::string script;
        
        while (true) {
            std::getline(std::cin, line);
            if (line == ".quit") {
                stop = true;
                break;
            } else if (line == ".run") {
                run(script);
                break;
            }
            // std::getline drops the \n, but we need it.
            script += line + "\n";
            
        }
    }
}

int main(int argc, const char * argv[]) {
    try {
        if (argc > 2) {
            std::print("Usage: cpplox [script]\n");
            return 64;
        } else if (argc == 2) {
            std::print("*** Running file: {}\n", argv[1]);
            run_file(argv[1]);
        } else {
            std::print("*** Running REPL\n");
            std::print(".run to run the script.\n");
            std::print(".quit to exit REPL.\n");
            run_prompt();
        }
    } catch (const std::exception& exc) {
        std::print("Caught exception: {}\n", exc.what());
        return 64;
    }
    
    return 0;
}



