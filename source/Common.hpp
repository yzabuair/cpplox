// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <string>
#include <variant>

namespace cpplox {
// Forwards.
class Environment;
struct LoxInstance;
struct LoxClass;

/// Anything that is callable must be stuffable into a std::function.
struct Callable {
    friend std::ostream& operator<<(std::ostream& stream, const Callable& callable);
    
    int arity{0};
    
    /// Note that std::function only works with things that are copyable, that means for example std::unique_ptr will not work in a std::function.
    std::function<std::any (const std::vector<std::any>&)> func;
    
};
inline std::ostream& operator<<(std::ostream& stream, const Callable& callable) {
    stream << "<native fn>";
    return stream;
}
using ValueType = std::variant<std::monostate, std::string, double, bool, nullptr_t, Callable, std::shared_ptr<LoxInstance>, std::shared_ptr<LoxClass>>;




} // namespace cpplox
