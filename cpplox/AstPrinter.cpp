// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "AstPrinter.hpp"

#include <sstream>

namespace cpplox {
std::string AstPrinter::print(Expr& expr) {
    expr.accept(*this);
    
    return stream_.str();
}

void AstPrinter::visit(const AssignExpr& expr) {
    
}

void AstPrinter::visit(const BinaryExpr& expr) {
    parenthesize_(expr.operation.lexeme, {*(expr.left.get()), *(expr.right.get())});
}

void AstPrinter::visit(const LiteralExpr& expr) {
    if (expr.value.index() == 0) {
        stream_ << "nil";
    }
    
    if (expr.value.index() == 1) {
        stream_ << std::get<std::string>(expr.value);
    } else {
        stream_ << std::to_string(std::get<double>(expr.value));
    }
}

void AstPrinter::visit(const GroupingExpr& expr) {
    parenthesize_("group", {*(expr.expression.get())});
}

void AstPrinter::visit(const UnaryExpr& expr) {
    parenthesize_(expr.operation.lexeme, {*(expr.right.get())});
}

void AstPrinter::parenthesize_(const std::string& name, const std::vector<std::reference_wrapper<Expr>>& exprs) {
    stream_ << "(" << name;
    
    for(Expr& curr: exprs) {
        stream_ << " ";
        curr.accept(*this);
    }
    
    stream_ << ")";
}

} // namespace cpplox
