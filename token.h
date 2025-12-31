#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <unordered_map>
using namespace std;
enum TokenType {
      // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  END_OF_FILE
};

static unordered_map<TokenType, string> print_map;

class Token{
    public:
    TokenType type;
    string lexeme;
    // this std:any is the closest thing i can think of to the prototypical Java Object
    any literal;
    int line;
    Token(){}
    
    Token(TokenType type, string lexeme, any literal, int line){
        this->type = type;
        this->lexeme = lexeme;
        this->literal = literal;
        this->line = line;
    }

    friend ostream& operator<<(ostream& out, Token& token){
        out << token.lexeme <<" "<< print_map[token.type] <<endl;
        return out;
    }
};