// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once

#include "TokenType.hpp"

#include <iostream>
#include <string>
#include <variant>

namespace cpplox {

using TokenValueType = std::variant<std::monostate, std::string, double, bool, nullptr_t>;

/// Represents a token we've scanned from the stream.
struct Token {
    
public:
    TokenType type = TokenType::UNDEFINED;
    std::string lexeme;
    TokenValueType literal;
    int line = 0;
    
public:
    friend std::ostream& operator<<(std::ostream& stream, const Token& token);
    Token(TokenType type,
          const std::string& lexeme,
          const TokenValueType& literal,
          int line): type{type},
                     lexeme{lexeme},
                     literal{literal},
                     line{line} {
    }
    
};

inline std::ostream& operator<<(std::ostream& stream, const Token& token) {
    switch (token.type) {
        case TokenType::STRING:
        break;
            
        case TokenType::NUMBER:
            stream << token.type << " " << token.lexeme << " " << std::get<double>(token.literal) << "\n";
            break;
            if (token.literal.index() == 0) {
                stream << token.type << " " << token.lexeme << "empty \n";
            } else if (token.literal.index() == 1) {
                stream << token.type << " " << token.lexeme << " " << std::get<std::string>(token.literal) << "\n";
            } else if (token.literal.index() == 2){
                stream << token.type << " " << token.lexeme << " " << std::get<double>(token.literal) << "\n";
            } else if (token.literal.index() == 3) {
                stream << token.type << " " << token.lexeme << " " << std::get<bool>(token.literal) << "\n";
            } else {
                stream << token.type << " " << token.lexeme << " " << "nil\n";
            }
        default:
            stream << token.type << " " << token.lexeme << "\n";
            break;
    }
        
    
    return stream;
}

} // namespace cpplox

