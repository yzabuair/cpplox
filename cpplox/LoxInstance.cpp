// Copyright 2025, Yasser Zabuair.  See LICENSE for details.

#include "LoxInstance.hpp"

#include "LoxClass.hpp"
#include "RuntimeError.hpp"

#include <sstream>

namespace cpplox {

ValueType LoxInstance::get(const Token& name) {
    auto itr = fields.find(name.lexeme);
    if (itr == fields.end()) {
        auto result = lox_class->find_method(name.lexeme);
        if (result.index() == 0) {
            std::stringstream stream;
            stream << "Field/method is unknown: " << name.lexeme;
            throw RuntimeError(stream.str());
        } else {
            return result;
        }
    }
    
    return itr->second;
}

void LoxInstance::set(const Token& name, const ValueType& value) {
    fields[name.lexeme] = value;
}

} // namespace cpplox
