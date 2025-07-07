// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include "Common.hpp"
#include "Environment.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

#include <any>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace cpplox {

// Forwards
class LoxInstance;

/// The interpreter that "executes" the AST nodes.
class Interpreter: public ExprVisitor,
                   public StmtVisitor {
private:
    Environment global_env_{nullptr};
    Environment* curr_env_ = nullptr;
    std::map<uintptr_t, int> locals_;
    bool return_called_ = false;
                       
public:
    ValueType value;
    
    Interpreter();
    void interpret(Expr& expr);
    void interpret(const std::vector<std::unique_ptr<Stmt>>& stmts);
                       
    void resolve(uintptr_t expr_ptr, int depth);
    
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
    void visit(const GetExpr& expr) override;
    void visit(const SetExpr& expr) override;
    void visit(const ThisExpr& expr) override;
    void visit(const SuperExpr& expr) override;
                       
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
    void visit(const ClassDeclStatement& stmt) override;
   
// Internal Helpers
private:
    void evaluate_(Expr& expr);
    void execute_(Stmt& stmt);
    ValueType lookup_variable_(const Token& name, uintptr_t expr_ptr);
    void execute_block_(const std::vector<std::unique_ptr<Stmt>>& statements,
                        Environment& env);
    bool is_thruthy_(const ValueType& value);
    bool is_equal_(const ValueType& a, const ValueType& b);
    void stringify_();
    Callable make_func_callable_(const std::shared_ptr<FunctionDeclStatement>& stmt,
                                 const std::shared_ptr<LoxInstance>& instance = nullptr,
                                 const std::shared_ptr<LoxClass>& super_class = nullptr);
};

} // namespace cpplox
