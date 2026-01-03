#include "interpreter.h"

string stringify(any literal){
    if(isNil(literal)) return "nil";
    if (instanceof(literal, double)){
        string text = to_string(any_cast<double>(literal));
        return text;
    }
    if (instanceof(literal, bool)){
        if (any_cast<bool>(literal) == true)
            return "true";
        return "false";
    }
    return any_cast<string>(literal);
}

void checkNumberOperand(Token op, any operand){
    if (instanceof(operand, double))
        return;
    throw runtimeException(op, "Operand must be a number.");
}

void checkNumberOperands(Token op, any left, any right){
    if (instanceof(left, double) && instanceof(right, double))
        return;
    throw runtimeException(op, "Operand must be numbers.");
}

any Interpreter::visit(Literal& expr){
    return expr.literal;
}

any Interpreter::visit(Grouping& expr){
    return evaluate(expr.expression);
}

any Interpreter::evaluate(Expr* expr){
    return expr->accept(this);
}

any Interpreter::visit(Unary& expr){
    any right = evaluate(expr.right);
    switch(expr.op.type){
        case MINUS:
            checkNumberOperand(expr.op, right);
            return -any_cast<double>(right);
        case BANG:
            return !isTruthy(right);
    }
    return NULL;
}

bool Interpreter::isTruthy(any literal){
    if (isNil(literal))
        return false;
    if (literal.type() == typeid(bool))
        return any_cast<bool>(literal);
    return true;
}

bool isEqual(any left, any right){
    if (isNil(left) && isNil(right))
        return true;
    if (isNil(left))
        return false;
    if (instanceof(left, double) && instanceof(right, double))
        return any_cast<double>(left) == any_cast<double>(right);
    if (instanceof(left, string) && instanceof(right, string))
        return any_cast<string>(left) == any_cast<string>(right);
}

any Interpreter::visit(Binary& expr){
    any left = evaluate(expr.left);
    any right = evaluate(expr.right);

    switch(expr.op.type){
        case MINUS:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left) - any_cast<double>(right);
        case STAR:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left) * any_cast<double>(right);
        case SLASH:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left)/any_cast<double>(right);
        case PLUS: 
            if (instanceof(left, double) && instanceof(right, double))
                return any_cast<double>(left) + any_cast<double>(right);
            if (instanceof(left, string) && instanceof(right, string))
                return any_cast<string>(left) + any_cast<string>(right);
            throw runtimeException(expr.op, "Operands must be two numbers or two strings.");
        case GREATER:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left) > any_cast<double>(right);
        case LESS:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left) < any_cast<double>(right);
        case GREATER_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left) >= any_cast<double>(right);
        case LESS_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return any_cast<double>(left) <= any_cast<double>(right);    
        case EQUAL_EQUAL:  
            return isEqual(left, right);
        case BANG_EQUAL:
            return !isEqual(left, right);
    }
}

void Interpreter::interpret(vector<Stmt*> statements){
    try{
        for (auto statement : statements)
            execute(statement);
    } catch(runtimeException exception){
        runtimeError(exception);
    }
}

any Interpreter::visit(Expression& expr){
    evaluate(expr.expression);
    return any();
}

any Interpreter::visit(Print& expr){
    any value = evaluate(expr.expression);
    cout << stringify(value);
    return any();
}

void Interpreter::execute(Stmt* stmt){
    stmt->accept(this);
}

any Interpreter::visit(Var& stmt){
    any value = any();
    if (stmt.expression != nullptr){
        value = evaluate(stmt.expression);
    }
    env.define(stmt.name.lexeme, value);
    return any();
}

any Interpreter::visit(Variable& expr){
    return env.get(expr.name);
}

any Interpreter::visit(Assign& expr){
    any value = evaluate(expr.value);
    env.assign(expr.name, value);
    return any();
}