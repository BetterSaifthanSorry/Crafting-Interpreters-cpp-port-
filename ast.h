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
class Expr;
class Expression;
class Print;
class Var;
class Variable;
class Assign;
#define instanceof(v, t) v.type() == typeid(t)
#define isNil(v) !v.has_value()
class Visitor{
    public:
    virtual any visit(Binary& expr) = 0;
    virtual any visit(Grouping& expr) = 0;
    virtual any visit(Literal& expr) = 0;
    virtual any visit(Unary& expr) = 0;
    virtual any visit(Variable& expr) = 0;
    virtual any visit(Assign& expr) = 0;
};

class StmtVisitor{
    public:
        virtual any visit(Expression& stmt) = 0;
        virtual any visit(Print& stmt) = 0;
        virtual any visit(Var& stmt) = 0;
};

class Stmt{
    public:
    virtual any accept(StmtVisitor* visitor) = 0;
};

class Expression : public Stmt{
    public:
    Expr* expression;
    Expression(Expr* expression){
        this->expression=expression;
    }
    any accept(StmtVisitor* visitor){
        return visitor->visit(*this);
    }
};

class Print : public Stmt{
    public:
    Expr* expression;
    Print(Expr* expression){
        this->expression=expression;
    }
    any accept(StmtVisitor* visitor){
        return visitor->visit(*this);
    }
};

class Var : public Stmt{
    public:
    Token name;
    Expr* expression;
    Var(Token name, Expr* expression){
        this->name=name;
        this->expression=expression;
    }
    any accept(StmtVisitor* visitor){
        return visitor->visit(*this);
    }
};

class Expr{
    public:
    virtual any accept(Visitor* visitor)=0;
};

class Variable : public Expr {
    public:
    Token name;
    Variable(Token name){
        this->name=name;
    }
    any accept(Visitor* visitor){
        return visitor->visit(*this);
    }
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

class Assign : public Expr {
    public:
    Token name;
    Expr* value;
    Assign(Token name, Expr* value){
        this->name=name;
        this->value=value;
    }
    any accept(Visitor* visitor){
        return visitor->visit(*this);
    }
};