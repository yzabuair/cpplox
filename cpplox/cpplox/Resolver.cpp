// Copyright 2025 Yasser Zabuair.  See LICENSE for details.
#include "Resolver.hpp"

#include "ParserError.hpp"

namespace cpplox {

void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& stmts) {
    for(const auto& stmt: stmts) {
        resolve_(stmt);
    }
}

void Resolver::visit(const AssignExpr& expr) {
    resolve_(expr.value);
    resolve_local_(expr, expr.name);
}

void Resolver::visit(const BinaryExpr& expr) {
    resolve_(expr.left);
    resolve_(expr.right);
}

void Resolver::visit(const LiteralExpr& expr) {
    // We do nothing in this case, no variables involved.
}

void Resolver::visit(const GroupingExpr& expr) {
    resolve_(expr.expression);
}

void Resolver::visit(const UnaryExpr& expr) {
    resolve_(expr.right);
}

void Resolver::visit(const VariableExpr& expr) {
    if (!scopes_.empty()) {
        auto itr = scopes_.front().find(expr.name.lexeme);
        if (itr != scopes_.front().end()) {
            if (itr->second == false) {
                throw ParserError("Can not read local variable in its own initializer.");
            }
        }
    }
    
    resolve_local_(expr, expr.name);
}

void Resolver::visit(const LogicalExpr& expr) {
    resolve_(expr.left);
    resolve_(expr.right);
}

void Resolver::visit(const CallExpr& expr) {
    resolve_(expr.callee);
    
    for(const auto& arg: expr.args) {
        resolve_(arg);
    }
}

void Resolver::visit(const PrintStatement& stmt) {
    resolve_(stmt.expression);
}

void Resolver::visit(const ExpressionStatement& stmt) {
    resolve_(stmt.expression);
}

void Resolver::visit(const VariableDeclStatement& stmt) {
    declare_(stmt.name);
    if (stmt.initializer) {
        resolve_(stmt.initializer);
    }
    
    define_(stmt.name);
}

void Resolver::visit(const BlockStatement& stmt) {
    begin_scope_();
    resolve(stmt.statements);
    end_scope_();
}

void Resolver::visit(const IfStatement& stmt) {
    resolve_(stmt.condition);
    resolve_(stmt.then_branch);
    if (stmt.else_branch) {
        resolve_(stmt.else_branch);
    }
}

void Resolver::visit(const WhileStatement& stmt) {
    resolve_(stmt.condition);
    resolve_(stmt.body);
}

void Resolver::visit(const FunctionDeclStatementProxy& stmt_proxy) {
    declare_(stmt_proxy.stmt->name);
    define_(stmt_proxy.stmt->name);
    resolve_function_(*(stmt_proxy.stmt.get()), FunctionType::Function);
}

void Resolver::visit(const ReturnStatement& stmt) {
    if (current_func == FunctionType::None) {
        throw ParserError("Can not return from top-level code.");
    }
    
    if (stmt.value) {
        resolve_(stmt.value);
    }
}

void Resolver::begin_scope_() {
    scopes_.push_front(std::map<std::string, bool>{});
}

void Resolver::resolve_(const std::unique_ptr<Stmt>& stmt) {
    stmt->accept(*this);
}

void Resolver::resolve_(const std::unique_ptr<Expr>& expr) {
    expr->accept(*this);
}

void Resolver::end_scope_() {
    scopes_.pop_front();
}

void Resolver::declare_(const Token& name) {
    if (scopes_.empty()) {
        return;
    }
    
    scopes_.front()[name.lexeme] = false;
}

void Resolver::define_(const Token& name) {
    if (scopes_.empty()) {
        return;
    }
    
    const auto& curr_scope = scopes_.front();
    if (curr_scope.find(name.lexeme) != curr_scope.end()) {
        std::stringstream stream;
        stream << "Variable " << name.lexeme << " is already defined.";
        throw ParserError(stream.str());
    }
    
    scopes_.front()[name.lexeme] = true;
}

void Resolver::resolve_local_(const Expr& expr, const Token& name) {
    int idx = 0;
    for(const auto& curr_scope: scopes_) {
        if (curr_scope.contains(name.lexeme)) {
            interpreter_.resolve(reinterpret_cast<uintptr_t>(&expr), idx);
            break;
        }
        ++idx;
    }
    
}

void Resolver::resolve_function_(const FunctionDeclStatement& stmt, const FunctionType& type) {
    auto enclosing_func = current_func;
    current_func = type;
    
    begin_scope_();
    for(const auto& curr_param: stmt.params) {
        declare_(curr_param);
        define_(curr_param);
    }
    resolve(stmt.body);
    end_scope_();
    
    current_func = enclosing_func;
}

} // namespace cpplox
