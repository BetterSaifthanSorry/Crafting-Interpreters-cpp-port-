#include "environment.h"
#include "src.h"

Environment::Environment(unordered_map<string, any> values){
    this->values=values;
}

void Environment::define(string name, any value){
    values[name] = value;
}

any Environment::get(Token name){
    if (values.count(name.lexeme)){
        return values[name.lexeme];
    }
    if (enclosing != nullptr)
        return enclosing->get(name);
    throw runtimeException(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, any value){
    if(values.count(name.lexeme)){
        values[name.lexeme] = value;
        return;
    }
    if (enclosing != nullptr){
        enclosing->assign(name, value);
        return;
    }
    throw runtimeException(name, "Undefined variable '" + name.lexeme + "'.");
}
    