// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "Environment.hpp"

#include "RuntimeError.hpp"

namespace cpplox {

void Environment::define(const std::string& name, const ValueType& value) {
    values_[name] = value;
}

const ValueType& Environment::get(const Token& name) const {
    auto itr = values_.find(name.lexeme);
    if (itr == values_.end()) {
        if (parent_ == nullptr) {
            std::stringstream stream;
            stream << "Undefined variable: " << name.lexeme;
            throw RuntimeError(stream.str());
        }
        
        return parent_->get(name);
    }
    
    return itr->second;
}

const ValueType& Environment::get_at(int distance, const Token& name) {
    return ancestor_(distance).values_[name.lexeme];
}

void Environment::assign(const Token& name, const ValueType& value) {
    auto itr = values_.find(name.lexeme);
    if (itr == values_.end()) {
        if (parent_ == nullptr) {
            std::stringstream stream;
            stream << "Undefined variable: " << name.lexeme;
            throw RuntimeError(stream.str());
        }
        
        return parent_->assign(name, value);
    }
    
    itr->second = value;
    
}

void Environment::assign_at(int distance,
                            const Token& name,
                            const ValueType& value) {
    ancestor_(distance).values_[name.lexeme] = value;
}

Environment& Environment::ancestor_(int distance) {
    Environment* curr = this;
    for(int i = 0; i < distance; ++i) {
        curr = curr->parent_;
    }
    
    return *curr;
}

} // namespace cpplox
