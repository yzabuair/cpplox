// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "LoxClass.hpp"

#include "RuntimeError.hpp"

#include <sstream>

namespace cpplox {

ValueType LoxClass::find_method(const std::string& method_name) {
    auto itr = methods.find(method_name);
    if (itr != std::end(methods)) {
        return itr->second;
    }
    
    if (super_class) {
        return super_class->find_method(method_name);
    }
    
    return nullptr;
}

} // namespace cpplox

