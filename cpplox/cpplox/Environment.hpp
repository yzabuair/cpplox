// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once
#include "Common.hpp"
#include "Token.hpp"

#include <any>
#include <map>
#include <memory>
#include <string>

namespace cpplox {

/// The execution environment of the script, will contain sub-environments for things such as functions and class methods.
class Environment {
private:
    std::map<std::string, ValueType> values_;
    Environment* parent_= nullptr;
    
public:
    Environment(Environment* parent): parent_{parent} {
    }
    
    Environment() {
        
    }
    
    void set_parent(Environment* parent) {
        parent_ = parent;
    }
    void define(const std::string& name, const ValueType& value);
    const ValueType& get(const Token& name) const;
    const ValueType& get_at(int distance, const std::string& name);
    void assign(const Token& name, const ValueType& value);
    void assign_at(int distance,
                   const Token& name,
                   const ValueType& value);
                 
    
private:
    Environment& ancestor_(int distance);
    
};

} // namespace cpplox
