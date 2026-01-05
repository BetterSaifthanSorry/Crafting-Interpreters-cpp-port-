#pragma once
#include"src.h"
#include"token.h"
#include"ast.h"

using namespace std;

class AstPrinter: public Visitor{
    public:
    any visit(Binary& expr);
    any visit(Grouping& expr);
    any visit(Literal& expr);
    any visit(Unary& expr);
    string parenthesize(string name, vector<Expr*> exprs);
    void print(Expr* expr);
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
    vector<Stmt*> parse(void);
    Stmt* statement(void);
    Stmt* expressionStatement(void);
    Stmt* printStatement(void);
    Stmt* declaration(void);
    Stmt* varDeclaration(void);
    Expr* assignment(void);
    vector<Stmt*> block(void);
};