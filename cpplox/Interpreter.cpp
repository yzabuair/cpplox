// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "Interpreter.hpp"

#include "LoxClass.hpp"
#include "LoxInstance.hpp"
#include "RuntimeError.hpp"

#include <chrono>
#include <functional>
#include <print>
#include <typeinfo>
#include <type_traits>


namespace cpplox {

Interpreter::Interpreter() {
    curr_env_ = &global_env_;
    Callable callable;
    callable.arity = 0;
    callable.func = [](const std::vector<std::any>&) -> std::any {
        // TODO: Calculate this properly.
        return ValueType{200.0};
    };
    
    global_env_.define("clock", callable);
}

void Interpreter::interpret(Expr& expr) {
    evaluate_(expr);
    stringify_();
}

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& stmts) {
    for(auto& curr: stmts) {
        execute_(*(curr.get()));
    }
}

void Interpreter::resolve(uintptr_t expr_ptr, int depth) {
    locals_[expr_ptr] = depth;
}

void Interpreter::visit(const AssignExpr& expr) {
    evaluate_(*(expr.value.get()));
    auto rhs = value;
    
    auto itr = locals_.find(reinterpret_cast<uintptr_t>(&expr));
    if (itr == locals_.end()) {
        global_env_.assign(expr.name, rhs);
    } else {
        curr_env_->assign_at(itr->second, expr.name, rhs);
    }
}

void Interpreter::visit(const BinaryExpr& expr) {
    evaluate_(*(expr.left.get()));
    auto lhs = value;
    
    evaluate_(*(expr.right.get()));
    auto rhs = value;
    
    switch (expr.operation.type) {
        case TokenType::GREATER:
            value = std::get<double>(lhs) > std::get<double>(rhs);
            break;
        case TokenType::GREATER_EQUAL:
            value = std::get<double>(lhs) >= std::get<double>(rhs);
            break;
        case TokenType::LESS:
            value = std::get<double>(lhs) < std::get<double>(rhs);
            break;
        case TokenType::LESS_EQUAL:
            value = std::get<double>(lhs) <= std::get<double>(rhs);
            break;
        case TokenType::MINUS:
            value = std::get<double>(lhs) - std::get<double>(rhs);
            break;
            
        case TokenType::SLASH:
            value = std::get<double>(lhs) - std::get<double>(rhs);
            break;
            
        case TokenType::STAR:
            value = std::get<double>(lhs) * std::get<double>(rhs);
            break;
            
        case TokenType::PLUS:
            if (lhs.index() == 2 &&
                rhs.index() == 2) {
                value = std::get<double>(lhs) + std::get<double>(rhs);
            } else if (lhs.index() == 1 &&
                       rhs.index() == 1) {
                value = std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }
            break;
            
        default:
            throw RuntimeError("Unknown operation");
            break;
    }
    
}

void Interpreter::visit(const LiteralExpr& expr) {
    switch (expr.value.index()) {
        case 0:
            value = nullptr;
        break;
        
        case 1:
            value = std::get<std::string>(expr.value);
        break;
            
        case 2:
            value = std::get<double>(expr.value);
        break;
            
        case 3:
            value = std::get<bool>(expr.value);
        break;
            
        case 4:
            value = nullptr;
        break;
            
        default:
            throw RuntimeError("Invalid value");
        break;
    }
}

void Interpreter::visit(const GroupingExpr& expr) {
    evaluate_(*(expr.expression.get()));
}

void Interpreter::visit(const UnaryExpr& expr) {
    evaluate_(*(expr.right.get()));
    auto rhs = value;
    
    switch (expr.operation.type) {
        case TokenType::MINUS:
            value =  -(std::get<double>(rhs));
            break;
        case TokenType::BANG:
            value = !is_thruthy_(rhs);
            break;
        default:
            // TODO: Error..
            break;
    }
}

void Interpreter::visit(const VariableExpr& expr) {
    value = lookup_variable_(expr.name, reinterpret_cast<uintptr_t>(&expr));
}

void Interpreter::visit(const LogicalExpr& expr) {
    evaluate_(*(expr.left.get()));
    auto left = value;
    
    if (expr.operation.type == TokenType::OR) {
        if (is_thruthy_(left)) {
            value = left;
        }
    } else {
        if (!is_thruthy_(left)) {
            value = left;
        }
    }
    
    evaluate_(*(expr.right.get()));
}

void Interpreter::visit(const CallExpr& expr) {
    evaluate_(*(expr.callee));
    auto callee = value;
    
    if (callee.index() != 5) {
        //
        // If we are not Callable and we're not a LoxClass, nothing we can do with this.
        //
        if (callee.index() != 7) {
            std::stringstream stream;
            stream << "This is not a callable object at line: " << expr.closing_paren.line;
            throw RuntimeError(stream.str());
        }
    }
    
    std::vector<std::any> args;
    for(auto& arg: expr.args) {
        evaluate_(*(arg.get()));
        args.push_back(std::move(value));
    }
    
    if (callee.index() == 5) {
        Callable function_info = std::get<Callable>(callee);
        if (args.size() != function_info.arity) {
            throw RuntimeError("Wrong number of args");
        }
        
        auto result = function_info.func(args);
        value = std::any_cast<ValueType>(result);
    } else {
        std::shared_ptr<LoxClass> lox_class = std::get<std::shared_ptr<LoxClass>>(callee);
        if (lox_class->initializer.arity != args.size()) {
            throw RuntimeError("Wrong number of args for initializer");
        }
        
        auto result = lox_class->initializer.func(args);
        value = std::any_cast<ValueType>(result);
    }
}

void Interpreter::visit(const GetExpr& expr) {
    evaluate_(*(expr.object.get()));
    ValueType object = value;
    
    if (object.index() != 6) {
        throw RuntimeError("Only object instances have properties.");
    }
    
    auto instance = std::get<std::shared_ptr<LoxInstance>>(object);
    value = instance->get(expr.name);
}

void Interpreter::visit(const SetExpr& expr) {
    evaluate_(*(expr.object.get()));
    ValueType object = value;
    
    if (object.index() != 6) {
        throw RuntimeError("Only object instances have properties.");
    }
    
    evaluate_(*(expr.value.get()));
    auto the_value = value;
    
    auto instance = std::get<std::shared_ptr<LoxInstance>>(object);
    instance->set(expr.name, the_value);
    
}

void Interpreter::visit(const ThisExpr& expr) {
    value = lookup_variable_(expr.keyword, reinterpret_cast<uintptr_t>(&expr));
}

void Interpreter::visit(const SuperExpr& expr) {
    auto itr = locals_.find(reinterpret_cast<uintptr_t>(&expr));
    if (itr == locals_.end()) {
        throw RuntimeError("Could not find 'super' in environment.");
    }
    
    auto super = curr_env_->get_at(itr->second, "super");
    auto super_class = std::get<std::shared_ptr<LoxClass>>(super);
    value = super_class->find_method(expr.method.lexeme);
}

void Interpreter::evaluate_(Expr& expr) {
    expr.accept(*this);
}

void Interpreter::execute_(Stmt& stmt) {
    stmt.accept(*this);
}

ValueType Interpreter::lookup_variable_(const Token& name, uintptr_t expr_ptr) {
    auto itr = locals_.find(expr_ptr);
    if (itr == locals_.end()) {
        return global_env_.get(name);
    } else {
        return curr_env_->get_at(itr->second, name.lexeme);
    }
}

// Makes sure environment gets setup correclty.
struct EnvGuard {
    Environment*& curr_env;
    Environment* original;
    EnvGuard(Environment*& curr_env,
             Environment* new_env): curr_env{curr_env} {
        this->original = curr_env;
        this->curr_env = new_env;

    }
    
    ~EnvGuard() {
        curr_env = original;
    }
};

void Interpreter::execute_block_(const std::vector<std::unique_ptr<Stmt>>& statements,
                                 Environment& env) {
    EnvGuard guard{curr_env_, &env};

    for(auto& statement: statements) {
        execute_(*(statement.get()));
        if (return_called_) {
            break;
        }
    }
}

void Interpreter::visit(const PrintStatement& stmt) {
    evaluate_(*(stmt.expression.get()));
    stringify_();
}

void Interpreter::visit(const ExpressionStatement& stmt) {
    evaluate_(*(stmt.expression.get()));
}

void Interpreter::visit(const VariableDeclStatement& stmt) {
    ValueType initial_value;
    
    if (stmt.initializer) {
        evaluate_(*(stmt.initializer.get()));
        initial_value = value;
    }
    
    curr_env_->define(stmt.name.lexeme, initial_value);
}

void Interpreter::visit(const BlockStatement& stmt) {
    Environment new_env{curr_env_};
    execute_block_(stmt.statements, new_env);
}

void Interpreter::visit(const IfStatement& stmt) {
    evaluate_(*(stmt.condition.get()));
    auto result = value;
    
    if (is_thruthy_(result)) {
        execute_(*(stmt.then_branch.get()));
    } else if (stmt.else_branch) {
        execute_(*(stmt.else_branch.get()));
    }
}

void Interpreter::visit(const WhileStatement& stmt) {
    evaluate_(*(stmt.condition.get()));
    while(is_thruthy_(value)) {
        execute_(*(stmt.body.get()));
        if (return_called_) {
            break;
        }
        evaluate_(*(stmt.condition.get()));
    }
}

void Interpreter::visit(const FunctionDeclStatementProxy& stmt_proxy) {
    curr_env_->define(stmt_proxy.stmt->name.lexeme, make_func_callable_(stmt_proxy.stmt));
}

void Interpreter::visit(const ReturnStatement& stmt) {
    if (stmt.value) {
        evaluate_(*(stmt.value.get()));
    } else {
        value = std::monostate();
    }
    
    return_called_ = true;
}

void Interpreter::visit(const ClassDeclStatement& stmt) {
    curr_env_->define(stmt.name.lexeme, nullptr);
    
    //
    // Handle super class if one is defined.
    //
    std::shared_ptr<LoxClass> super_class;
    if (stmt.super_class) {
        evaluate_(*(stmt.super_class));
        auto evaulated_super_class = value;
        
        if (evaulated_super_class.index() != 7) {
            throw RuntimeError("The superclass is not a class.");
        } else {
            super_class = std::get<std::shared_ptr<LoxClass>>(evaulated_super_class);
        }
    }
    
    //
    // Sets up the methods in the class.
    //
    auto lox_instance = LoxInstance::create();
    
    std::map<std::string, ValueType> methods;
    std::shared_ptr<FunctionDeclStatement> init_method;
    for(const auto& curr: stmt.methods) {
        methods[curr->name.lexeme] = make_func_callable_(curr, lox_instance, super_class);
        if (curr->name.lexeme == "init") {
            init_method = curr;
        }
    }
    
    auto lox_class = LoxClass::create(stmt.name.lexeme, methods, super_class);
    lox_instance->lox_class = lox_class;
    
    //
    // Sets up initializer that will create class instance and initialize.
    //
    Callable init_call;
    if (init_method) {
        init_call =  make_func_callable_(init_method, lox_instance);
    } else {
        init_call.arity = 0;
        init_call.func = [this, lox_instance](const std::vector<std::any>& params) -> std::any {
            Environment env{curr_env_};
            
            ValueType instance =  lox_instance;
            return instance;
        };
    }
    
    lox_class->initializer = std::move(init_call);
    
    curr_env_->assign(stmt.name, lox_class);
}

bool Interpreter::is_thruthy_(const ValueType& value) {
    if (value.index() == 0) {
        return false;
    }
    
    if (value.index() == 3) {
        bool result = std::get<bool>(value);
        if (!result) {
            return false;
        }
    }
    
    return true;
}

bool Interpreter::is_equal_(const ValueType& a, const ValueType& b) {
    switch (a.index()) {
        case 0:
            return false;
            
        case 1:
            return std::get<1>(a) == std::get<1>(b);
            
        case 2:
            return std::get<2>(a) == std::get<2>(b);
            
        case 3:
            return std::get<3>(a) == std::get<3>(b);
            
        case 4:
            return std::get<4>(a) == std::get<4>(b);
            
        default:
            // TODO: Error...
            break;
    }
    
    
    return false;
}

void Interpreter::stringify_() {
    switch (value.index()) {
        case 0:
            std::print("nil\n");
            break;
            
        case 1:
            std::print("{}\n", std::get<1>(value));
            break;
            
        case 2:
            std::print("{}\n", std::get<2>(value));
            break;
            
        case 3:
            std::print("{}\n", std::get<3>(value));
            break;
            
        case 4:
            std::print("{}\n", std::get<4>(value));
            break;
            
        case 5:
            std::cout << std::get<Callable>(value) << "\n";
            break;
            
        case 6:
            std::cout << std::get<std::shared_ptr<LoxInstance>>(value)->lox_class->name << "\n";
            break;
            
        default:
            std::print("Unknown value type\n");
            break;
    }

}

Callable Interpreter::make_func_callable_(const std::shared_ptr<FunctionDeclStatement>& stmt,
                                          const std::shared_ptr<LoxInstance>& instance,
                                          const std::shared_ptr<LoxClass>& super_class) {
    Callable callable;
    callable.arity = static_cast<int>(stmt->params.size());
    callable.func = [this, stmt, instance, super_class](const std::vector<std::any>& params) -> std::any {
        Environment env;
        Environment class_env{curr_env_};
        
        //
        // If there is an instance, we are setting up a class method so setup environment properly.
        //
        if (instance) {
            class_env.define("this", instance);
            if (super_class) {
                class_env.define("super", super_class);
            }
            env.set_parent(&class_env);
        } else {
            env.set_parent(curr_env_);
        }

        for(int i = 0; i < params.size(); ++i) {
            env.define(stmt->params[i].lexeme, std::any_cast<ValueType>(params[i]));
        }

        return_called_ = false;
        value = std::monostate();

        execute_block_(stmt->body, env);

        if (stmt->name.lexeme == "init") {
            if (return_called_) {
                throw RuntimeError("Return makes no sense in an initializer.");
            }
            value = instance;
        } else {
            // If the function just ends with no return, the result is nil.
            if (!return_called_) {
                value = std::monostate();
            }
        }

        return value;
    };
    return callable;
}

} // namespace cpplox
