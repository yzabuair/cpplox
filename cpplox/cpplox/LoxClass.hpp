// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include "Common.hpp"

#include <map>
#include <memory>
#include <string>

namespace cpplox {

/// Represents a class in lox, which is primarily a containter for the methods and creates new instances.
struct LoxClass {
    std::string name;
    std::map<std::string, ValueType> methods;
    Callable initializer;
    std::shared_ptr<LoxClass> super_class;

    LoxClass(const std::string& name,
             const std::map<std::string, ValueType>& methods,
             const std::shared_ptr<LoxClass> super_class):
        name{name},
        methods{methods},
        super_class{super_class} {
    }
    
    static std::shared_ptr<LoxClass> create(const std::string& name,
                                            const std::map<std::string, ValueType>& methods,
                                            const std::shared_ptr<LoxClass> super_class) {
        return std::make_shared<LoxClass>(name, methods, super_class);
    }
    
    ValueType find_method(const std::string& method_name);
};

} // namespace cpplox
