// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "Parser.hpp"

#include "ParserError.hpp"

namespace cpplox {

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    auto statements = std::vector<std::unique_ptr<Stmt>>();
    
    while(!is_at_end_()) {
        statements.push_back(declaration_());
    }
    
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration_() {
    if (match_({TokenType::VAR})) {
        return var_declaration_();
    }
    
    if (match_({TokenType::FUN})) {
        return function_decl_statement_("function");
    }
    
    if (match_({TokenType::CLASS})) {
        return class_decl_statement_();
    }
    
    return statement_();
}

std::unique_ptr<Stmt> Parser::statement_() {
    if (match_({TokenType::PRINT})) {
        return print_statement_();
    }
    
    if (match_({TokenType::LEFT_BRACE})) {
        return BlockStatement::create(block_());
    }
    
    if (match_({TokenType::IF})) {
        return if_statement_();
    }
    
    if (match_({TokenType::WHILE})) {
        return while_statement_();
    }
    
    if (match_({TokenType::FOR})) {
        return for_statement_();
    }
    
    if (match_({TokenType::RETURN})) {
        return return_statement_();
    }
    
    return expression_statement_();
}

std::unique_ptr<Stmt> Parser::if_statement_() {
    consume_(TokenType::LEFT_PAREN, "Expect '(' after if.");
    std::unique_ptr<Expr> condition = expression_();
    consume_(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    
    auto then_branch = statement_();
    std::unique_ptr<Stmt> else_branch;
    if (match_({TokenType::ELSE})) {
        else_branch = statement_();
    }
    
    return IfStatement::create(std::move(condition), std::move(then_branch), std::move(else_branch));
    
}

std::unique_ptr<Stmt> Parser::for_statement_() {
    consume_(TokenType::LEFT_PAREN, "Expect '(' after for.");
    
    std::unique_ptr<Stmt> initializer;
    if (match_({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match_({TokenType::VAR})) {
        initializer = var_declaration_();
    } else {
        initializer = expression_statement_();
    }
    
    std::unique_ptr<Expr> condition;
    if (!check_(TokenType::SEMICOLON)) {
        condition = expression_();
    }
    
    consume_(TokenType::SEMICOLON, "Expect ';' after loop condition.");
    
    std::unique_ptr<Expr> increment;
    if (!check_(TokenType::RIGHT_PAREN)) {
        increment = expression_();
    }
    
    consume_(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
    
    std::unique_ptr<Stmt> body = statement_();
    
    if (increment) {
        std::vector<std::unique_ptr<Stmt>> stmts;
        stmts.push_back(std::move(body));
        stmts.push_back(ExpressionStatement::create(std::move(increment)));
        
        body = BlockStatement::create(std::move(stmts));
    }
    
    if (!condition) {
        condition = LiteralExpr::create(true);
    }
    
    body = WhileStatement::create(std::move(condition), std::move(body));
    
    if (initializer) {
        std::vector<std::unique_ptr<Stmt>> stmts;
        stmts.push_back(std::move(initializer));
        stmts.push_back(std::move(body));
        body = BlockStatement::create(std::move(stmts));
    }
    
    return body;
}

std::unique_ptr<Stmt> Parser::print_statement_() {
    auto expr = expression_();
    consume_(TokenType::SEMICOLON, "Expect ';' after print.");
    
    return PrintStatement::create(std::move(expr));
}

std::unique_ptr<Stmt> Parser::return_statement_() {
    Token keyword = previous_();
    
    std::unique_ptr<Expr> value;
    if (!check_(TokenType::SEMICOLON)) {
        value = expression_();
    }
    
    consume_(TokenType::SEMICOLON, "Expect ';' after return value.");
    
    return ReturnStatement::create(keyword, std::move(value));
}

std::unique_ptr<Stmt> Parser::var_declaration_() {
    auto name = consume_(TokenType::IDENTIFIER, "Expected variable name.");
    
    std::unique_ptr<Expr> initializer;
    if (match_({TokenType::EQUAL})) {
        initializer = expression_();
    }
    
    consume_(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    
    return VariableDeclStatement::create(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::while_statement_() {
    consume_(TokenType::LEFT_PAREN, "Expect '(' after 'while.'");
    
    auto condition = expression_();
    
    consume_(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    
    auto body = statement_();
    
    return WhileStatement::create(std::move(condition), std::move(body));
    
}

std::unique_ptr<Stmt> Parser::expression_statement_() {
    auto expr = expression_();
    consume_(TokenType::SEMICOLON, "Expect ';' after expression.");
    
    return ExpressionStatement::create(std::move(expr));
}

std::unique_ptr<Stmt> Parser::function_decl_statement_(const std::string& kind) {
    Token name = consume_(TokenType::IDENTIFIER, "Expect " + kind + " name");
    consume_(TokenType::LEFT_PAREN, "Expect '('" + kind + " after name.");
    std::vector<Token> params;
    if (!check_(TokenType::RIGHT_PAREN)) {
        if (params.size() >= 255) {
            throw ParserError("Too many parameters.", name);
        }
        
        do {
            params.push_back(consume_(TokenType::IDENTIFIER, "Exepect param name."));
        } while (match_({TokenType::COMMA}));
        
    }
    consume_(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consume_(TokenType::LEFT_BRACE, "Exepect '{' before " + kind + " body.");
    
    auto body = block_();
    
    return FunctionDeclStatementProxy::create(name, params, std::move(body));
}

std::unique_ptr<Stmt> Parser::class_decl_statement_() {
    Token name = consume_(TokenType::IDENTIFIER, "Expect class name");
    std::unique_ptr<VariableExpr> super_class;
    if (match_({TokenType::LESS})) {
        consume_(TokenType::IDENTIFIER, "Expect superclass name.");
        super_class = VariableExpr::create(previous_());
    }
    consume_(TokenType::LEFT_BRACE, "Expect '{' before class body.");
    
    std::vector<std::shared_ptr<FunctionDeclStatement>> methods;
    while(!check_(TokenType::RIGHT_BRACE) && !is_at_end_()) {
        auto stmt = function_decl_statement_("method");
        auto proxy = dynamic_cast<FunctionDeclStatementProxy*>(stmt.get());
        
        methods.push_back(proxy->stmt);
    }
    
    consume_(TokenType::RIGHT_BRACE, "Expect '}' after body.");
    
    return ClassDeclStatement::create(name, std::move(super_class), std::move(methods));
}

std::vector<std::unique_ptr<Stmt>> Parser::block_() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while(!check_(TokenType::RIGHT_BRACE) && !is_at_end_()) {
        statements.push_back(declaration_());
    }
    
    consume_(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    
    return statements;
}

std::unique_ptr<Expr> Parser::expression_() {
    return assignment_();
}

std::unique_ptr<Expr> Parser::assignment_() {
    auto expr = or_();
    
    if (match_({TokenType::EQUAL})) {
        Token equals = previous_();
        auto value = assignment_();
        
        if (dynamic_cast<VariableExpr*>(expr.get())) {
            auto name = dynamic_cast<VariableExpr*>(expr.get())->name;
            return AssignExpr::create(name, std::move(value));
        } else if (dynamic_cast<GetExpr*>(expr.get())) {
            auto get_expr = dynamic_cast<GetExpr*>(expr.get());
            return SetExpr::create(std::move(get_expr->object), get_expr->name, std::move(value));
        }
        
        throw ParserError("Invalid assignment target.", equals);
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::or_() {
    auto expr = and_();
    
    while(match_({TokenType::OR})) {
        Token operation = previous_();
        auto right = and_();
        return LogicalExpr::create(std::move(expr), std::move(operation), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::and_() {
    auto expr = equality_();
    
    while(match_({TokenType::AND})) {
        Token operation = previous_();
        auto right = equality_();
        return LogicalExpr::create(std::move(expr), std::move(operation), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::equality_() {
    auto expr = comparison_();
    
    while (match_({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        auto operation = previous_();
        auto right = comparison_();
        
        expr = BinaryExpr::create(std::move(expr), operation, std::move(right));
        
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::comparison_() {
    auto expr = term_();
    
    while(match_({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        auto operation = previous_();
        auto right = term_();
        expr = BinaryExpr::create(std::move(expr), operation, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::term_() {
    auto expr = factor_();
    
    while(match_({TokenType::MINUS, TokenType::PLUS})) {
        auto operation = previous_();
        auto right = factor_();
        expr = BinaryExpr::create(std::move(expr), operation, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::factor_() {
    auto expr = unary_();
    
    while(match_({TokenType::SLASH, TokenType::STAR})) {
        auto operation = previous_();
        auto right = unary_();
        expr = BinaryExpr::create(std::move(expr), operation, std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::unary_() {
    if (match_({TokenType::BANG, TokenType::MINUS})) {
        auto operation = previous_();
        auto right = unary_();
        return UnaryExpr::create(operation, std::move(right));
    }
    
    return call_();
}

std::unique_ptr<Expr> Parser::call_() {
    std::unique_ptr<Expr> expr = primary_();
    
    while (true) {
        if (match_({TokenType::LEFT_PAREN})) {
            expr = finish_call_(std::move(expr));
        } else if (match_({TokenType::DOT})) {
            Token name = consume_(TokenType::IDENTIFIER, "Expect property name after '.'");
            expr = GetExpr::create(std::move(expr), name);
        } else {
            break;
        }
    }
    
    return expr;
}

std::unique_ptr<Expr> Parser::finish_call_(std::unique_ptr<Expr> callee) {
    std::vector<std::unique_ptr<Expr>> args;
    
    if (!check_(TokenType::RIGHT_PAREN)) {
        do {
            if (args.size() >= 255) {
                throw ParserError("Too many args!", peek_());
            }
            args.push_back(expression_());
        } while (match_({TokenType::COMMA}));
    }
    
    Token paren = consume_(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    
    return CallExpr::create(std::move(callee), paren, std::move(args));
}

std::unique_ptr<Expr> Parser::primary_() {
    if (match_({TokenType::LEFT_PAREN})) {
        auto expr = expression_();
        consume_(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return GroupingExpr::create(std::move(expr));
    }
    
    if (match_({TokenType::FALSE})) {
        return LiteralExpr::create({false});
    }
    
    if (match_({TokenType::TRUE})) {
        return LiteralExpr::create({true});
    }
    
    if (match_({TokenType::NIL})) {
        return LiteralExpr::create(nullptr);
    }
    
    if (match_({TokenType::STRING, TokenType::NUMBER})) {
        return LiteralExpr::create({previous_().literal});
    }
    
    if (match_({TokenType::THIS})) {
        return ThisExpr::create(previous_());
    }
    
    if (match_({TokenType::SUPER})) {
        Token keyword = previous_();
        consume_(TokenType::DOT, "Expect '.' after 'super'.");
        Token method = consume_(TokenType::IDENTIFIER, "Expect superclass method name.");
        
        return SuperExpr::create(keyword, method);
    }
    
    if (match_({TokenType::IDENTIFIER})) {
        return VariableExpr::create(previous_());
    }
    
    throw ParserError("Expect expression", peek_());
}



bool Parser::match_(const std::vector<TokenType>& match_types) {
    for(auto curr_type: match_types) {
        if (check_(curr_type)) {
            advance_();
            return true;
        }
    }
    
    return false;
    
}

bool Parser::check_(TokenType token_type) {
    if (is_at_end_()) {
        return false;
    }
    
    return peek_().type == token_type;
}

const Token& Parser::advance_() {
    if (is_at_end_()) {
        return tokens.back();
    }
    
    return tokens[current++];
}

bool Parser::is_at_end_() {
    return peek_().type == TokenType::ENDOFFILE;
}

const Token& Parser::peek_() {
    return tokens[current];
}

const Token& Parser::previous_() {
    return tokens[current-1];
}

const Token& Parser::consume_(TokenType type, const std::string& message, std::source_location location) {
    if (check_(type)) {
        return advance_();
    }
    
    throw ParserError(message, previous_(), location);
}


} // namespace cpplox
