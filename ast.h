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