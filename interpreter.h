#pragma once
#include "token.h"
#include "parse.h"
#include "src.h"
#include <typeinfo>
#include "ast.h"
using namespace std;
class Visitor;
class Interpreter:public Visitor{
    public:
        any visit(Binary& expr);
        any visit(Grouping& expr);
        any visit(Literal& expr);
        any visit(Unary& expr);
        any evaluate(Expr* expr);
        bool isTruthy(any literal);
        void interpret(Expr* expression);
};