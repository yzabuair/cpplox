// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include <exception>
#include <source_location>
#include <sstream>
#include <string>

namespace cpplox {

struct ScannerError: public std::exception {
    
private:
    std::string          inp_message_;
    int                  script_line_{0};
    mutable std::string  message_;
    std::source_location caller_location_;
    
public:
    ScannerError(const std::string& msg,
                 int script_line,
                 std::source_location location = std::source_location::current()):
        inp_message_{msg},
        script_line_{script_line},
        caller_location_{location} {
    }
    
// std::exception
public:
    inline const char* what() const noexcept override {
        if (message_.empty()) {
            std::stringstream stream;
            stream << "ScannerError, at line, " << script_line_ << " error: " << inp_message_ << "\n";
            stream << "In file: " << caller_location_.file_name() << ", line: " << caller_location_.line() << "\n";
            message_ = stream.str();
            return message_.c_str();
        }
        
        return message_.c_str();
    }
};

} // namespace cpplox

