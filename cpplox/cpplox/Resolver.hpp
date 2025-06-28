// Copyright 2025, See LICENSE for details.
#pragma once

#include "Expr.hpp"
#include "Interpreter.hpp"
#include "Stmt.hpp"

#include <map>
#include <stack>
#include <string>
#include <vector>

namespace cpplox {

class Resolver: public ExprVisitor,
                public StmtVisitor {
                    
private:
    enum class FunctionType {
        None,
        Function
    };
    Interpreter& interpreter_;
    std::deque<std::map<std::string, bool>> scopes_;
    FunctionType current_func = FunctionType::None;
                    
public:
    Resolver(Interpreter& interpreter):
        interpreter_{interpreter} {
    }
                    
    void resolve(const std::vector<std::unique_ptr<Stmt>>& stmts);

// ExprVisitor Implementation
public:
    void visit(const AssignExpr& expr) override;
    void visit(const BinaryExpr& expr) override;
    void visit(const LiteralExpr& expr) override;
    void visit(const GroupingExpr& expr) override;
    void visit(const UnaryExpr& expr) override;
    void visit(const VariableExpr& expr) override;
    void visit(const LogicalExpr& expr) override;
    void visit(const CallExpr& expr) override;
                    
// StmtVisitor Implementation
public:
    void visit(const PrintStatement& stmt) override;
    void visit(const ExpressionStatement& stmt) override;
    void visit(const VariableDeclStatement& stmt) override;
    void visit(const BlockStatement& stmt) override;
    void visit(const IfStatement& stmt) override;
    void visit(const WhileStatement& stmt) override;
    void visit(const FunctionDeclStatementProxy& stmt_proxy) override;
    void visit(const ReturnStatement& stmt) override;
                    
// Internal Helpers
private:
    void begin_scope_();
    void resolve_(const std::unique_ptr<Stmt>& stmt);
    void resolve_(const std::unique_ptr<Expr>& expr);
    void end_scope_();
    void declare_(const Token& name);
    void define_(const Token& name);
    void resolve_local_(const Expr& expr, const Token& name);
    void resolve_function_(const FunctionDeclStatement& stmt, const FunctionType& type);
};

} // namespace cpplox
