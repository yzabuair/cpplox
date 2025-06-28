// Copyright 2025, Yasser Zabuair.  See LICENSE for details.
#pragma once
#include <iostream>

namespace cpplox {


enum class TokenType {
    UNDEFINED,
    
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    
    // One or two character tokens.
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    
    // Literals.
    IDENTIFIER, STRING, NUMBER,
    
    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    
    NEWLINE, ENDOFFILE
};

inline std::ostream& operator<<(std::ostream& ostream, const TokenType& type) {
    switch (type) {
    case TokenType::UNDEFINED:
        ostream << "UNDEFINED";
        break;
    case TokenType::LEFT_PAREN:
        ostream << "LEFT_PAREN";
        break;
    case TokenType::RIGHT_PAREN:
        ostream << "RIGHT_PAREN";
        break;
    case TokenType::LEFT_BRACE:
        ostream << "LEFT_BRACE";
        break;
    case TokenType::RIGHT_BRACE:
        ostream << "RIGHT_BRACE";
        break;
    case TokenType::COMMA:
        ostream << "COMMA";
        break;
    case TokenType::DOT:
        ostream << "DOT";
        break;
    case TokenType::MINUS:
        ostream << "MINUS";
        break;
    case TokenType::PLUS:
        ostream << "PLUS";
        break;
    case TokenType::SEMICOLON:
        ostream << "SEMICOLON";
        break;
    case TokenType::SLASH:
        ostream << "SLASJ";
        break;
    case TokenType::STAR:
        ostream << "STAR";
        break;
    case TokenType::BANG:
        ostream << "!";
        break;
    case TokenType::BANG_EQUAL:
        ostream << "!=";
        break;
    case TokenType::EQUAL:
        ostream << "=";
    case TokenType::EQUAL_EQUAL:
        ostream << "==";
    case TokenType::GREATER:
        ostream << ">";
        break;
    case TokenType::GREATER_EQUAL:
        ostream << ">=";
        break;
    case TokenType::LESS:
        ostream << "<";
        break;
    case TokenType::LESS_EQUAL:
        ostream << "<=";
        break;
    case TokenType::IDENTIFIER:
        ostream << "IDENTIFIER";
        break;
    case TokenType::NUMBER:
        ostream << "NUMBER";
        break;
    case TokenType::STRING:
        ostream << "STRING";
        break;
    case TokenType::AND:
        ostream << "AND";
        break;
    case TokenType::CLASS:
        ostream << "CLASS";
        break;
    case TokenType::ELSE:
        ostream << "ELSE";
        break;
    case TokenType::FALSE:
        ostream << "FALSE";
        break;
    case TokenType::FUN:
        ostream << "FUN";
        break;
    case TokenType::FOR:
        ostream << "FOR";
        break;
    case TokenType::IF:
        ostream << "IF";
        break;
    case TokenType::NIL:
        ostream << "NIL";
        break;
    case TokenType::OR:
        ostream << "OR";
        break;
    case TokenType::PRINT:
        ostream << "PRINT";
        break;
    case TokenType::RETURN:
        ostream << "RETURN";
        break;
    case TokenType::SUPER:
        ostream << "SUPER";
        break;
    case TokenType::THIS:
        ostream << "THIS";
        break;
    case TokenType::TRUE:
        ostream << "TRUE";
        break;
    case TokenType::VAR:
        ostream << "VAR";
        break;
    case TokenType::WHILE:
        ostream << "WHILE";
        break;
    case TokenType::ENDOFFILE:
        ostream << "EOF";
        break;
    default:
        ostream << "UNKNOWN token";
        break;
    }
    
    return ostream;
}


} // namespace cpplox

