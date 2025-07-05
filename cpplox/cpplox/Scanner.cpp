// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#include "Scanner.hpp"

#include "ScannerError.hpp"


namespace cpplox {

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end_()) {
        start_ = current_;
        scan_token_();
    }
    
    tokens_.push_back(Token{TokenType::ENDOFFILE, "", {}, line_});
    
    return std::move(tokens_);
}

bool Scanner::is_at_end_() {
    return current_ >= source_.size();
}

void Scanner::scan_token_() {
    char c = advance_();
    
    switch (c) {
        case '(': add_token_(TokenType::LEFT_PAREN); break;
        case ')': add_token_(TokenType::RIGHT_PAREN); break;
        case '{': add_token_(TokenType::LEFT_BRACE); break;
        case '}': add_token_(TokenType::RIGHT_BRACE); break;
        case ',': add_token_(TokenType::COMMA); break;
        case '.': add_token_(TokenType::DOT); break;
        case '-': add_token_(TokenType::MINUS); break;
        case '+': add_token_(TokenType::PLUS); break;
        case ';': add_token_(TokenType::SEMICOLON); break;
        case '*': add_token_(TokenType::STAR); break;
        case '!':
            if (match_('=')) {
                add_token_(TokenType::BANG_EQUAL);
            } else {
                add_token_(TokenType::BANG);
            }
            break;
        case '=':
            if (match_('=')) {
                add_token_(TokenType::EQUAL_EQUAL);
            } else {
                add_token_(TokenType::EQUAL);
            }
            break;
        case '<':
            if (match_('=')) {
                add_token_(TokenType::LESS_EQUAL);
            } else {
                add_token_(TokenType::LESS);
            }
            break;
        case '>':
            if (match_('=')) {
                add_token_(TokenType::GREATER_EQUAL);
            } else {
                add_token_(TokenType::GREATER);
            }
            break;
        case '/':
            if (match_('/')) {
                while (peek_() != '\n' && peek_() != '\r' && !is_at_end_()) {
                    advance_();
                }
            } else if (match_('*')) {
                eat_multi_line_comment_();
            } else {
                add_token_(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            ++line_;
            break;
        case '"':
            string_();
            break;
        default:
            if (std::isdigit(c)) {
                number_();
            } else if (is_alpha_(c)) {
                identifier_();
            } else {
                throw ScannerError("Unexpected token", line_);
            }
            break;
    }
}

char Scanner::advance_() {
    if (current_ >= source_.size()) {
        throw ScannerError("You tried to go past end of script", line_);
    }
    
    return source_[current_++];
}

void Scanner::add_token_(TokenType type) {
    add_token_(type, cpplox::TokenValueType{});
}

void Scanner::add_token_(TokenType type, const cpplox::TokenValueType& literal_value) {
    auto text = source_.substr(start_, (current_ - start_));
    tokens_.push_back(Token(type, text, literal_value, line_));
}

bool Scanner::match_(char expected) {
    if (is_at_end_()) {
        return false;
    }
    
    if (source_[current_] != expected) {
        return false;
    }
    
    ++current_;
    return true;
}

char Scanner::peek_() {
    if (is_at_end_()) {
        return '\0';
    }
    
    return source_[current_];
}

char Scanner::peek_next_() {
    if (is_at_end_()) {
        return '\0';
    }
    
    if (current_ + 1 >= source_.size()) {
        return '\0';
    }
    
    return source_[current_ + 1];
}

void Scanner::string_() {
    while (peek_() != '"' && !is_at_end_()) {
        if (peek_() == '\n') {
            ++line_;
        }
        advance_();
    }
    
    // We hit end of script, but no closing ".
    if (is_at_end_()) {
        throw ScannerError("Unclosed string", line_);
    }

    // Go past "
    advance_();
    
    std::string string_value = source_.substr(start_ + 1, (current_ - start_) - 2);
    add_token_(TokenType::STRING, string_value);
    

}

void Scanner::number_() {
    while(std::isdigit(peek_())) {
        advance_();
    }
    
    if (peek_() == '.' && std::isdigit(peek_next_())) {
        advance_();
        
        while(std::isdigit(peek_())) {
            advance_();
        }
    }
    
    std::string number_string = source_.substr(start_, current_ - start_);
    double number_value = std::stod(number_string);
    
    add_token_(TokenType::NUMBER, number_value);
}

bool Scanner::is_alpha_(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::is_alpha_numeric_(char c) {
    return is_alpha_(c) || std::isdigit(c);
}

void Scanner::identifier_() {
    while (is_alpha_numeric_(peek_())) {
        advance_();
    }
    
    std::string identifier = source_.substr(start_, current_ - start_);
    auto itr = keywords_.find(identifier);
    if (itr == keywords_.end()) {
        add_token_(TokenType::IDENTIFIER, identifier);
    } else {
        add_token_(itr->second);
    }
}

void Scanner::eat_multi_line_comment_() {
    // Go past *.
    advance_();
    
    bool found_end_of_comment = false;
    while(!is_at_end_() && !found_end_of_comment) {
        char c = peek_();
        
        switch (c) {
            case '*':
                if (peek_next_() == '/') {
                    advance_();
                    advance_();
                    found_end_of_comment = true;
                } else {
                    advance_();
                }
                break;
            case '/':
                // This is a comment within a comment.
                if (peek_next_() == '*') {
                    eat_multi_line_comment_();
                } else {
                    advance_();
                }
                break;
            case '\n':
                ++line_;
            default:
                // Eat it.
                advance_();
                break;
        }
    }
    
    if (is_at_end_() && !found_end_of_comment) {
        throw ScannerError("You did not end comment.", line_);
    }
}

} // namespace cpplox
