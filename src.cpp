#include "src.h"

using namespace std;
bool hadError = false;
bool hadRuntimeError = false;

void report(int line, string where, string msg){
    cout << "[ line " << line << "] Error" << where << ": " << msg << endl;
    hadError = true;
}

void runtimeError(runtimeException exception){
    cout << exception.what() << "\n[ line" << exception.token.line << "]"<<endl;
    hadRuntimeError = true;
}

void error(Token token, string message){
    if (token.type == END_OF_FILE) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void error(int line, string msg){
    report(line, "", msg);
}


unordered_map<string, TokenType> hash_map;


class Scanner{
    string source;
    vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    public:
    Scanner(string src){
        this->source=src;
    }

    vector<Token>& scanTokens(void){
        while(!isAtEnd()){
            start = current;
            scanToken();
        }
        tokens.push_back(Token(END_OF_FILE, "", 0, line));
        return tokens;
    }

    bool isAtEnd(void){
        return current >= source.length();
    }

    void scanToken(void){
        char c = advance();
        switch(c){
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;
            case '!': addToken(match('=')? BANG_EQUAL : BANG); break;
            case '=': addToken(match('=')? EQUAL_EQUAL : EQUAL); break;
            case '<': addToken(match('=')? LESS_EQUAL : LESS); break;
            case '>': addToken(match('=')? GREATER_EQUAL : GREATER); break;
            case '/': 
                if (match('/')){
                    while(peek() != '\n' && !isAtEnd()) advance();
                }
                else{
                    addToken(SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n': line++; break;
            case '"': handleString(); break;
            default: 
                if(isDigit(c)){
                    handleNumber();
                    break;
                }
                else if (isAlpha(c)){
                    handleIdentifier();
                }
                else{
                    error(line, "Unexpected character"); 
                }
                break;
        }
    }

    bool isAlpha(char c){
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool isAlphaNumeric(char c){
        return isDigit(c) || isAlpha(c);
    }

    void handleIdentifier(void){
        while(isAlphaNumeric(peek())) advance();

        TokenType type = IDENTIFIER;
        string text = source.substr(start, current - start);
        if(hash_map.count(text)) 
            type = hash_map[text];
        addToken(type);
    }

    void handleNumber(void){
        while(isDigit(peek())) advance();

        if (peek() == '.' && isDigit(peekNext())){
            advance();
            while(isDigit(peek())) advance();
        }
        addToken(NUMBER, stod(source.substr(start, current - start)));
    }

    bool isDigit(char c){
        return c >= '0' && c <= '9';
    }

    char peekNext(void){
        if (current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }

    char peek(void){
        if (isAtEnd()) return '\0';
        return source[current];
    }

    void handleString(void){
        while(peek() != '"' && !isAtEnd()){
            if (peek()=='\n') line++;
            advance();
        }

        if(isAtEnd()){
            error(line, "Unterminated string");
            return;
        }

        advance();

        string val = source.substr(start+1, (current-1)-(start+1));
        addToken(STRING, val);
    }

    bool match(char expected){
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }

    char advance(void){
        return source[current++];
    }

    void addToken(TokenType type){
        addToken(type, 0);
    }

    void addToken(TokenType type, any literal){
        string text = source.substr(start, current-start);
        tokens.push_back(Token(type, text, literal, line));
    }
};

void run(string line){
    Scanner scanner(line);
    static Interpreter interpreter;
    vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    // Expr* expr = parser.parse();
    vector<Stmt*> statements = parser.parse();

    if (hadError) return;
    interpreter.interpret(statements);
    // AstPrinter printer;
    // printer.print(expr);
}

void runFile(char* path){
    string prog = "";
    string line = "";
    ifstream f(path);
    while (getline(f, line)){
        prog+=(line = line + "\n");
    }
    run(prog);
}

int main(int argc, char** argv){

    hash_map["and"] = AND;
    hash_map["class"] = CLASS;
    hash_map["else"] = ELSE;
    hash_map["false"] = FALSE;
    hash_map["for"] = FOR;
    hash_map["fun"] = FUN;
    hash_map["if"] = IF;
    hash_map["nil"] = NIL;
    hash_map["or"] = OR;
    hash_map["print"] = PRINT;
    hash_map["return"] = RETURN;
    hash_map["super"] = SUPER;
    hash_map["this"] = THIS;
    hash_map["true"] = TRUE;
    hash_map["var"] = VAR;
    hash_map["while"] = WHILE;
 
    print_map[LEFT_PAREN] = "LEFT_PAREN";
    print_map[RIGHT_PAREN] = "RIGHT_PAREN";
    print_map[LEFT_BRACE] = "LEFT_BRACE";
    print_map[RIGHT_BRACE] = "RIGHT_BRACE";
    print_map[COMMA] = "COMMA";
    print_map[DOT] = "DOT";
    print_map[MINUS] = "MINUS";
    print_map[PLUS] = "PLUS";
    print_map[SEMICOLON] = "SEMICOLON";
    print_map[SLASH] = "SLASH";
    print_map[STAR] = "STAR";
    print_map[BANG] = "BANG";
    print_map[BANG_EQUAL] = "BANG_EQUAL";
    print_map[EQUAL] = "EQUAL";
    print_map[EQUAL_EQUAL] = "EQUAL_EQUAL";
    print_map[GREATER] = "GREATER";
    print_map[GREATER_EQUAL] = "GREATER_EQUAL";
    print_map[LESS] = "LESS";
    print_map[LESS_EQUAL] = "LESS_EQUAL";
    print_map[IDENTIFIER] = "IDENTIFIER";
    print_map[STRING] = "STRING";
    print_map[NUMBER] = "NUMBER";
    print_map[AND] = "AND";
    print_map[CLASS] = "CLASS";
    print_map[ELSE] = "ELSE";
    print_map[FALSE] = "FALSE";
    print_map[FUN] = "FUN";
    print_map[FOR] = "FOR";
    print_map[IF] = "IF";
    print_map[NIL] = "NIL";
    print_map[OR] = "OR";
    print_map[PRINT] = "PRINT";
    print_map[RETURN] = "RETURN";
    print_map[SUPER] = "SUPER";
    print_map[TRUE] = "TRUE";
    print_map[VAR] = "VAR";
    print_map[WHILE] = "WHILE";
    print_map[END_OF_FILE] = "END_OF_FILE";

    if (argc > 1)
        runFile(argv[1]);

    else{
    for(;;){
        cout << "> ";
        string line;
        if(!getline(cin, line))
            break;
        run(line);
        cout << endl;
        hadError=false;
    }
    }
}