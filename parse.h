#pragma once
#include"src.h"
#include"token.h"

using namespace std;

class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor;

class Visitor{
    public:
    virtual any visit(Binary& expr) = 0;
    virtual any visit(Grouping& expr) = 0;
    virtual any visit(Literal& expr) = 0;
    virtual any visit(Unary& expr) = 0;
};

class AstPrinter: public Visitor{
    public:
    any visit(Binary& expr);
    any visit(Grouping& expr);
    any visit(Literal& expr);
    any visit(Unary& expr);
    string parenthesize(string name, vector<Expr*> exprs);
    void print(Expr* expr);
};
class Expr{
    public:
    virtual any accept(Visitor* visitor)=0;
};

class Binary : public Expr {
    public:
    Expr* left;
    Token op;
    Expr* right;
    Binary(Expr* left, Token op, Expr* right){
        this->left=left;
        this->op=op;
        this->right=right;
    }
    any accept(Visitor* visitor){
        return visitor->visit(*this);
    }
};

class Grouping : public Expr {
    public:
    Expr* expression;
    Grouping(Expr* expression){
        this->expression=expression;
    }
    any accept(Visitor* visitor){
        return visitor->visit(*this);
    }
};

class Literal : public Expr{
    public:
    any literal;
    Literal(any literal){
        this->literal=literal;
    }
    any accept(Visitor* visitor){
        return visitor->visit(*this);
    }
};

class Unary : public Expr{
    public:
    Token op;
    Expr* right;
    Unary(Token op, Expr* right){
        this->op=op;
        this->right=right;
    }
    any accept(Visitor* visitor){
        return visitor->visit(*this);
    }
};

class ParseError : public runtime_error{
    public:
    ParseError() : runtime_error("error"){
        
    }
};

class Parser{
    vector<Token> tokens;
    int current = 0;
    public:
    Parser(vector<Token>& tokens);
    Expr* expression(void);
    Expr* equality(void);
    Expr* comparision(void);
    Expr* term(void);
    Expr* factor(void);
    Expr* unary(void);
    Expr* primary(void);
    bool match(const vector<TokenType>& tokens);
    bool check(TokenType type);
    Token advance(void);
    bool isAtEnd(void);
    Token peek(void);
    Token previous(void);
    Token consume(TokenType type, string msg);
    ParseError error(Token token, string msg);
    void synchronize(void);
    Expr* parse(void);
};