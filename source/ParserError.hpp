// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include <exception>
#include <source_location>
#include <sstream>
#include <string>

namespace cpplox {

/// Error thrown when we run into an error.
struct ParserError: public std::exception {
  
private:
    std::string             inp_message_;
    Token                   token_;
    std::source_location    caller_location_;
    mutable std::string     message_;
    
public:
    ParserError(const std::string& msg,
                const Token& token,
                std::source_location location = std::source_location::current()):
        inp_message_{msg},
        token_{token},
        caller_location_{location} {
    }
    
    
// std::exception
public:
    inline const char* what() const noexcept override {
        if (message_.empty()) {
            std::stringstream stream;
            stream << "ParserError: " << inp_message_ << "  In line: " << token_.line << " at token: " << token_.lexeme << "\n";
            stream << "In file: " << caller_location_.file_name() << ", line: " << caller_location_.line() << "\n";
            message_ = stream.str();
            return message_.c_str();
        }
        
        return message_.c_str();
    }
};

} // namespace cpplox

