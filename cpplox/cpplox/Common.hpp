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

struct Callable {
    friend std::ostream& operator<<(std::ostream& stream, const Callable& callable);
    
    int arity{0};
    std::function<std::any (const std::vector<std::any>&)> func;
    
};
inline std::ostream& operator<<(std::ostream& stream, const Callable& callable) {
    stream << "<native fn>";
    return stream;
}
using ValueType = std::variant<std::monostate, std::string, double, bool, nullptr_t, Callable>;


} // namespace cpplox
