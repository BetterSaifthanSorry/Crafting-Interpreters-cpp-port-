#pragma once
#include "token.h"
#include "environment.h"
#include "parse.h"
#include "src.h"
#include <typeinfo>
#include "ast.h"
using namespace std;
class Visitor;
class Interpreter:public Visitor, public StmtVisitor{
    public:
        Environment env;
        Interpreter(void){

        }
        using Visitor::visit;
        any visit(Binary& expr);
        any visit(Grouping& expr);
        any visit(Literal& expr);
        any visit(Unary& expr);
        any visit(Variable& expr);
        any visit(Assign& expr);
        any visit(Expression& stmt);
        any visit(Print& stmt);
        any visit(Var& stmt);
        any evaluate(Expr* expr);
        bool isTruthy(any literal);
        void interpret(vector<Stmt*> statements);
        void execute(Stmt* stmt);
};