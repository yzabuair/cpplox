// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include "Common.hpp"
#include "Token.hpp"

#include <map>
#include <memory>
#include <string>

namespace cpplox {

// Forwards
class LoxClass;

/// An instance of a Lox class.  Primarily this is where the state lives.
struct LoxInstance {
    std::shared_ptr<LoxClass> lox_class;
    std::map<std::string, ValueType> fields;
    
    
    static std::shared_ptr<LoxInstance> create() {
        return std::make_shared<LoxInstance>();
    }
    
    ValueType get(const Token& name);
    void set(const Token& name, const ValueType& value);
};

} // namespace cpplox
