// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once
#include <exception>
#include <source_location>
#include <sstream>
#include <string>

namespace cpplox {

/// Errors that occur when script is running.
struct RuntimeError: public std::exception {
    
private:
    std::string             inp_message_;
    std::source_location    caller_location_;
    mutable std::string     message_;
    
public:
    RuntimeError(const std::string& msg,
                 std::source_location location = std::source_location::current()):
        inp_message_{msg},
        caller_location_{location} {
    }
    
// std::exception
public:
    inline const char* what() const noexcept override {
        if (message_.empty()) {
            std::stringstream stream;
            stream << "RuntimeError: " << inp_message_ << "\n";
            stream << "In file: " << caller_location_.file_name() << ", at line: " << caller_location_.line() << "\n";
            message_ = stream.str();
            return message_.c_str();
        }
        
        return message_.c_str();
    }
};

} // namespace cpplox

