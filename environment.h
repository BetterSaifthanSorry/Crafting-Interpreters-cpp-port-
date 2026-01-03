#pragma once
#include<unordered_map>
#include<any>
#include<string>
#include "token.h"
using namespace std;

class Environment{
    public:
    Environment(){}
    unordered_map<string, any> values;
    Environment(unordered_map<string, any> values);
    void define(string name, any value);
    any get(Token name);
    void assign(Token name, any value);
};