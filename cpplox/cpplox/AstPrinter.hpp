// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include "Expr.hpp"

#include <functional>
#include <sstream>
#include <string>


namespace cpplox {

struct AstPrinter: public ExprVisitor {
public:
    std::string print(Expr& expr);
    virtual void visit(const AssignExpr& expr);
    virtual void visit(const BinaryExpr& expr);
    virtual void visit(const LiteralExpr& expr);
    virtual void visit(const GroupingExpr& expr);
    virtual void visit(const UnaryExpr& expr);
    
private:
    void parenthesize_(const std::string& name, const std::vector<std::reference_wrapper<Expr>>& exprs);
    std::stringstream stream_;
};
} // namespace cpplox
