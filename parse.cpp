#include"parse.h"

string AstPrinter::parenthesize(string name, vector<Expr*> exprs){
    string res = "";
    res += '(';
    res += name;
    for (auto expr : exprs){
        res += ' ';
        res += any_cast<string>(expr->accept(this));
    }
    res += ')';
    return res;
}

void AstPrinter::print(Expr* expr){
    cout << any_cast<string>(expr->accept(this))<<endl;
}

any AstPrinter::visit(Binary& expr){
    return parenthesize(expr.op.lexeme, {expr.left, expr.right});
}

any AstPrinter::visit(Grouping& expr){
    return parenthesize(std::string("group"), {expr.expression});
}

any AstPrinter::visit(Literal& expr){
    if (string* ptr = any_cast<string>(&expr.literal)) {
        return *ptr;
    } else if(double* ptr = any_cast<double>(&expr.literal)){    
        return to_string(*ptr);
    }
    else
        return string("nil");
}

any AstPrinter::visit(Unary& expr){
    return parenthesize(expr.op.lexeme, {expr.right});
}

Parser::Parser(vector<Token>& tokens){
    this->tokens=tokens;
}

Expr* Parser::expression(void){
    return assignment();
}

Expr* Parser::assignment(void){
    Expr* expr = equality();
    if (match({EQUAL})){
        Token equals = previous();
        Expr* value = assignment();

        if (dynamic_cast<Variable*>(expr)){
            Token name = dynamic_cast<Variable*>(expr)->name;
            return new Assign(name, value);
        }

        error(equals, "Invalid assignment target.");
    }
    return expr;
}

Expr* Parser::equality(void){
    Expr* expr = comparision();
    while(match({BANG_EQUAL, EQUAL_EQUAL})){
        Token op = previous();
        Expr* right = comparision();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

bool Parser::match(const vector<TokenType>& tokens){
    for (auto& token : tokens){
        if (check(token)){
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type){
    if(this->isAtEnd()) return false;
    return this->peek().type == type; 
}

Token Parser::advance(void){
    if (!(this->isAtEnd())) current++;
    return previous();
}

bool Parser::isAtEnd(void){
    return (this->peek()).type == END_OF_FILE;
}
Token Parser::peek(void){
    return tokens[current];
}
Token Parser::previous(void){
    return tokens[current-1];
}

Expr* Parser::comparision(void){
    Expr* expr = term();
    while(match({LESS, LESS_EQUAL, GREATER, GREATER_EQUAL})){
        Token op = previous();
        Expr* right = term();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::term(void){
    Expr* expr = factor();
    while(match({MINUS, PLUS})){
        Token op = previous();
        Expr* right = factor();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::factor(void){
    Expr* expr = unary();
    while(match({SLASH, STAR})){
        Token op = previous();
        Expr* right = unary();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::unary(void){
    if(match({BANG, MINUS})){
        Token op = previous();
        Expr* right = unary();
        return new Unary(op, right);
    }
    return primary();
}

Expr* Parser::primary(void){
    if (match({FALSE})) return new Literal(false);
    if (match({TRUE})) return new Literal(true);
    if (match({NIL})) return new Literal(any());

    if (match({NUMBER, STRING})){
        return new Literal(previous().literal);
    }

    if (match({IDENTIFIER})){
        return new Variable(previous());
    }

    if (match({LEFT_PAREN})){
        Expr* expr = expression();
        consume(RIGHT_PAREN, "Expect ) after expression");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression");
}

Token Parser::consume(TokenType type, string msg){
    if (check(type)) return advance();
    throw error(peek(), msg);
}

ParseError Parser::error(Token token, string msg){
    ::error(token, msg);
    return ParseError();
}

void Parser::synchronize(void){
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;

        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }

        advance();
    }
}

Stmt* Parser::statement(void){
    if (match({PRINT})) return printStatement();
    return expressionStatement();
}

Stmt* Parser::printStatement(void){
    Expr* value = expression();
    consume(SEMICOLON, "Expect ; after value.");
    return new Print(value);
}

Stmt* Parser::expressionStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return new Expression(expr);
}

vector<Stmt*> Parser::parse(void){
    vector<Stmt*> statements;
    while(!isAtEnd())
        statements.push_back(declaration());
    return statements;
}

Stmt* Parser::declaration(void){
    try{
        if (match({VAR})) return varDeclaration();
        return statement();
    }catch(ParseError error){
        synchronize();
        return nullptr;
    }
}

Stmt* Parser::varDeclaration(void){
    Token name = consume(IDENTIFIER, "Expect variable name.");

    Expr* initializer = nullptr;
    if (match({EQUAL})){
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer);
}
