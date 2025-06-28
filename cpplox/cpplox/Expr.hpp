// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once
#include "Token.hpp"

#include <memory>
#include <vector>

namespace cpplox {

// Forward Declarations
struct AssignExpr;
struct BinaryExpr;
struct LiteralExpr;
struct GroupingExpr;
struct UnaryExpr;
struct VariableExpr;
struct LogicalExpr;
struct CallExpr;

struct ExprVisitor {
    virtual void visit(const AssignExpr& expr) = 0;
    virtual void visit(const BinaryExpr& expr) = 0;
    virtual void visit(const LiteralExpr& expr) = 0;
    virtual void visit(const GroupingExpr& expr) = 0;
    virtual void visit(const UnaryExpr& expr) = 0;
    virtual void visit(const VariableExpr& expr) = 0;
    virtual void visit(const LogicalExpr& expr) = 0;
    virtual void visit(const CallExpr& expr) = 0;
};

struct Expr {
    Expr() {
    }
    
    virtual ~Expr() {
    }
    
    virtual void accept(ExprVisitor& visit) = 0;
};

// ---

struct AssignExpr: public Expr {
    Token name;
    std::unique_ptr<Expr> value;
    
    AssignExpr(const Token& name,
               std::unique_ptr<Expr> value): name{name},
                                             value{std::move(value)} {
    }
    
    static std::unique_ptr<AssignExpr> create(const Token& name,
                                              std::unique_ptr<Expr> value) {
        auto ptr = std::make_unique<AssignExpr>(name, std::move(value));
        
        return ptr;
    }
    
    void accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

// ---

struct LiteralExpr: public Expr {
    TokenValueType value;
    
    LiteralExpr(const TokenValueType& value): value{value} {
        
    }
    
    static std::unique_ptr<LiteralExpr> create(const TokenValueType& value) {
        auto ptr = std::make_unique<LiteralExpr>(value);
        
        return ptr;
    }
    
    void accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

// ---

struct BinaryExpr: public Expr {
    std::unique_ptr<Expr> left;
    Token operation;
    std::unique_ptr<Expr> right;
    
    BinaryExpr(std::unique_ptr<Expr> left,
               const Token& operation,
               std::unique_ptr<Expr> right): left{std::move(left)},
                                             operation{operation},
                                             right{std::move(right)} {
        
    }
    
    static std::unique_ptr<BinaryExpr> create(std::unique_ptr<Expr> left,
                                              const Token& operation,
                                              std::unique_ptr<Expr> right) {
        auto ptr = std::make_unique<BinaryExpr>(std::move(left),
                                                operation,
                                                std::move(right));
        return ptr;
    }
    
    
    void accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

// ---

struct GroupingExpr: public Expr {
    std::unique_ptr<Expr> expression;
    
    GroupingExpr(std::unique_ptr<Expr> expression): expression{std::move(expression)} {
    }
    
    static std::unique_ptr<GroupingExpr> create(std::unique_ptr<Expr> expression) {
        auto ptr = std::make_unique<GroupingExpr>(std::move(expression));
        
        return ptr;
    }
    
    void accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

// ---

struct UnaryExpr: public Expr {
    Token operation;
    std::unique_ptr<Expr> right;
    
    UnaryExpr(const Token& operation,
              std::unique_ptr<Expr> right):
        operation{operation},
        right{std::move(right)} {
    }
    
    static std::unique_ptr<UnaryExpr> create(const Token& token,
                                             std::unique_ptr<Expr> right) {
        auto ptr = std::make_unique<UnaryExpr>(token, std::move(right));
        
        return ptr;
    }
    
    void accept(ExprVisitor& visitor) override {
        return visitor.visit(*this);
    }
};

// ---

struct VariableExpr: public Expr {
    Token name;
    
    VariableExpr(const Token& name): name{name} {
        
    }
    
    static std::unique_ptr<VariableExpr> create(const Token& name) {
        return std::make_unique<VariableExpr>(name);
    }
    
    void accept(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct LogicalExpr: public Expr {
    std::unique_ptr<Expr> left;
    Token operation;
    std::unique_ptr<Expr> right;
    
    LogicalExpr(std::unique_ptr<Expr> left,
                Token operation,
                std::unique_ptr<Expr> right):
        left{std::move(left)},
        operation{std::move(operation)},
        right{std::move(right)} {
        
    }
    
    static std::unique_ptr<LogicalExpr> create(std::unique_ptr<Expr> left,
                                               Token operation,
                                               std::unique_ptr<Expr> right) {
        return std::make_unique<LogicalExpr>(std::move(left), std::move(operation), std::move(right));
    }
    
    void accept(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};

// ---

struct CallExpr: public Expr {
    std::unique_ptr<Expr>   callee;
    Token closing_paren;
    std::vector<std::unique_ptr<Expr>> args;
    
    CallExpr(std::unique_ptr<Expr> callee,
             const Token& closing_paren,
             std::vector<std::unique_ptr<Expr>> args):
        callee{std::move(callee)},
        closing_paren{closing_paren},
        args{std::move(args)} {
        
    }
    
    static std::unique_ptr<CallExpr> create(std::unique_ptr<Expr> callee,
                                            const Token& closing_paren,
                                            std::vector<std::unique_ptr<Expr>> args) {
        return std::make_unique<CallExpr>(std::move(callee), closing_paren, std::move(args));
    }
    
    void accept(ExprVisitor& visitor) override {
        visitor.visit(*this);
    }
    
    
};

} // namespace cpplox

