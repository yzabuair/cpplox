// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include <memory>
#include <vector>

#include "Expr.hpp"

namespace cpplox {

// Forward Declarations
struct PrintStatement;
struct ExpressionStatement;
struct VariableDeclStatement;
struct BlockStatement;
struct IfStatement;
struct WhileStatement;
struct FunctionDeclStatementProxy;
struct ReturnStatement;
struct ClassDeclStatement;

struct StmtVisitor {
    virtual void visit(const PrintStatement& stmt) = 0;
    virtual void visit(const ExpressionStatement& stmt) = 0;
    virtual void visit(const VariableDeclStatement& stmt) = 0;
    virtual void visit(const BlockStatement& stmt) = 0;
    virtual void visit(const IfStatement& stmt) = 0;
    virtual void visit(const WhileStatement& stmt) = 0;
    virtual void visit(const FunctionDeclStatementProxy& stmt_proxy) = 0;
    virtual void visit(const ReturnStatement& stmt) = 0;
    virtual void visit(const ClassDeclStatement& stmt) = 0;
};

struct Stmt {
    Stmt(){ }
    virtual ~Stmt(){ }
    
    virtual void accept(StmtVisitor& visit) = 0;
};

// ---

struct PrintStatement: public Stmt {
    std::unique_ptr<Expr>   expression;
    
    PrintStatement(std::unique_ptr<Expr> expr): expression{std::move(expr)} {
    }
    
    static std::unique_ptr<PrintStatement> create(std::unique_ptr<Expr> expression) {
        return std::make_unique<PrintStatement>(std::move(expression));
    }
    
    virtual void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct ExpressionStatement: public Stmt {
    std::unique_ptr<Expr>   expression;
    
    ExpressionStatement(std::unique_ptr<Expr> expr): expression(std::move(expr)) {
    }
    
    static std::unique_ptr<ExpressionStatement> create(std::unique_ptr<Expr> expr) {
        return std::make_unique<ExpressionStatement>(std::move(expr));
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct VariableDeclStatement: public Stmt {
    Token  name;
    std::unique_ptr<Expr> initializer;
    
    VariableDeclStatement(const Token& name,
                          std::unique_ptr<Expr> initializer):
        name{name},
        initializer{std::move(initializer)} {
    }
    
    static std::unique_ptr<VariableDeclStatement> create(const Token& name,
                                                         std::unique_ptr<Expr> initializer) {
        return std::make_unique<VariableDeclStatement>(name, std::move(initializer));
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct BlockStatement: public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    BlockStatement(std::vector<std::unique_ptr<Stmt>> statements): statements{std::move(statements)} {
        
    }
    
    static std::unique_ptr<BlockStatement> create(std::vector<std::unique_ptr<Stmt>> statements) {
        return std::make_unique<BlockStatement>(std::move(statements));
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct IfStatement: public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> then_branch;
    std::unique_ptr<Stmt> else_branch;
    
    IfStatement(std::unique_ptr<Expr> condition,
                std::unique_ptr<Stmt> then_branch,
                std::unique_ptr<Stmt> else_branch):
        condition{std::move(condition)},
        then_branch{std::move(then_branch)},
        else_branch{std::move(else_branch)} {
    }
    
    static std::unique_ptr<IfStatement> create(std::unique_ptr<Expr> condition,
                                               std::unique_ptr<Stmt> then_branch,
                                               std::unique_ptr<Stmt> else_branch) {
        return std::make_unique<IfStatement>(std::move(condition), std::move(then_branch), std::move(else_branch));
    }

    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct WhileStatement: public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
    
    WhileStatement(std::unique_ptr<Expr> condition,
                   std::unique_ptr<Stmt> body):
        condition{std::move(condition)},
        body{std::move(body)} {
        
    }
    
    static std::unique_ptr<WhileStatement> create(std::unique_ptr<Expr> condition,
                                                  std::unique_ptr<Stmt> body) {
        return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct FunctionDeclStatement: public std::enable_shared_from_this<FunctionDeclStatement> {
    Token                               name;
    std::vector<Token>                  params;
    std::vector<std::unique_ptr<Stmt>>  body;
    
    FunctionDeclStatement(const Token& name,
                          std::vector<Token> params,
                          std::vector<std::unique_ptr<Stmt>> body):
        name{name},
        params{std::move(params)},
        body{std::move(body)} {
    }
    
    static std::shared_ptr<FunctionDeclStatement> create(const Token& name,
                                                         const std::vector<Token>& params,
                                                         std::vector<std::unique_ptr<Stmt>> body) {
        return std::make_shared<FunctionDeclStatement>(name, params, std::move(body));
    }
        
    
};

struct FunctionDeclStatementProxy: public Stmt {
    std::shared_ptr<FunctionDeclStatement> stmt;
    
    FunctionDeclStatementProxy(const std::shared_ptr<FunctionDeclStatement>& stmt): stmt{stmt} {
    }
    
    static std::unique_ptr<FunctionDeclStatementProxy> create(const Token& name,
                                                              const std::vector<Token>& params,
                                                              std::vector<std::unique_ptr<Stmt>> body) {
        auto func_decl = FunctionDeclStatement::create(name, params, std::move(body));
        return std::make_unique<FunctionDeclStatementProxy>(func_decl);
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
    
};

// ---

struct ReturnStatement: public Stmt {
    Token                   keyword;
    std::unique_ptr<Expr>   value;
    
    ReturnStatement(const Token& keyword,
                    std::unique_ptr<Expr> value):
        keyword{keyword},
        value{std::move(value)} {
    }
    
    static std::unique_ptr<ReturnStatement> create(const Token& keyword,
                                                   std::unique_ptr<Expr> value) {
        return std::make_unique<ReturnStatement>(keyword, std::move(value));
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct ClassDeclStatement: public Stmt {
    Token name;
    std::unique_ptr<VariableExpr> super_class;
    std::vector<std::shared_ptr<FunctionDeclStatement>> methods;
    
    ClassDeclStatement(const Token& name,
                       std::unique_ptr<VariableExpr> super_class,
                       std::vector<std::shared_ptr<FunctionDeclStatement>> methods):
        name{name},
        super_class{std::move(super_class)},
        methods{std::move(methods)} {
    }
    
    static std::unique_ptr<ClassDeclStatement> create(const Token& name,
                                                      std::unique_ptr<VariableExpr> super_class,
                                                      std::vector<std::shared_ptr<FunctionDeclStatement>> methods) {
        return std::make_unique<ClassDeclStatement>(name, std::move(super_class), std::move(methods));
    }
    
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

} // namespace cpplox
