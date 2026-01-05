#pragma once
#include<unordered_map>
#include<any>
#include<string>
#include "token.h"
using namespace std;

class Environment{
    public:
    Environment* enclosing;
    unordered_map<string, any> values;
    Environment(){enclosing=nullptr;}
    Environment(Environment& other){
        this->enclosing = &other;
    }
    Environment(unordered_map<string, any> values);
    void define(string name, any value);
    any get(Token name);
    void assign(Token name, any value);
};