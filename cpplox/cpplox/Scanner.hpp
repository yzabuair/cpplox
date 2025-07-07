// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once
#include "Token.hpp"
#include "TokenType.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace cpplox {

/// Used to break-up the stream into tokens that we use for parsing.
class Scanner {
private:
    std::string source_;
    std::vector<Token> tokens_;
    int start_ = 0;
    int current_ = 0;
    int line_ = 1;
    const std::map<std::string, TokenType> keywords_ = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
    };
    
    
public:
    Scanner(const std::string& source):
        source_{source} {
    }
    
    Scanner(std::istream& stream) {
        
    }
    
    std::vector<Token> scan_tokens();
    
private:
    bool is_at_end_();
    void scan_token_();
    char advance_();
    void add_token_(TokenType type);
    void add_token_(TokenType type, const cpplox::TokenValueType& literal_value);
    bool match_(char expected);
    char peek_();
    char peek_next_();
    void string_();
    void number_();
    bool is_alpha_(char c);
    bool is_alpha_numeric_(char c);
    void identifier_();
    void eat_multi_line_comment_();
};

} // namespace cpplox
