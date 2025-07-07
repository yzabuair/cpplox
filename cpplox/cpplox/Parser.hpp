// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include "Expr.hpp"
#include "Stmt.hpp"
#include "Token.hpp"

#include <memory>
#include <source_location>
#include <vector>

namespace cpplox {

/// Parses tokens into AST nodes.
struct Parser {
    std::vector<Token> tokens;
    int current = 0;
    
    std::vector<std::unique_ptr<Stmt>> parse();

private:

    std::unique_ptr<Stmt> declaration_();
    std::unique_ptr<Stmt> statement_();
    std::unique_ptr<Stmt> for_statement_();
    std::unique_ptr<Stmt> if_statement_();
    std::unique_ptr<Stmt> print_statement_();
    std::unique_ptr<Stmt> return_statement_();
    std::unique_ptr<Stmt> var_declaration_();
    std::unique_ptr<Stmt> while_statement_();
    std::unique_ptr<Stmt> expression_statement_();
    std::unique_ptr<Stmt> function_decl_statement_(const std::string& kind);
    std::unique_ptr<Stmt> class_decl_statement_();
    std::vector<std::unique_ptr<Stmt>> block_();
    std::unique_ptr<Expr> expression_();
    std::unique_ptr<Expr> assignment_();
    std::unique_ptr<Expr> or_();
    std::unique_ptr<Expr> and_();
    std::unique_ptr<Expr> equality_();
    std::unique_ptr<Expr> comparison_();
    std::unique_ptr<Expr> term_();
    std::unique_ptr<Expr> factor_();
    std::unique_ptr<Expr> unary_();
    std::unique_ptr<Expr> call_();
    std::unique_ptr<Expr> finish_call_(std::unique_ptr<Expr> callee);
    std::unique_ptr<Expr> primary_();
    bool match_(const std::vector<TokenType>& match_types);
    bool check_(TokenType token_type);
    const Token& advance_();
    bool is_at_end_();
    const Token& peek_();
    const Token& previous_();
    const Token& consume_(TokenType type, const std::string& message, std::source_location location = std::source_location::current());
};
} // namespace cpplox
