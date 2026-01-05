#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <any>
#include <unordered_map>
#include "parse.h"
#include "token.h"
#include "interpreter.h"

using namespace std;
class runtimeException : public runtime_error{
    public:
    Token token;
    runtimeException(Token token, string msg) : runtime_error(msg){
        this->token=token;
    }
};
void error(Token token, string message);
void runtimeError(runtimeException exception);

