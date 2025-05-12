#include <iostream>
#include <regex>
#include <string>
#include <map>
#include "lex.h"

using namespace std;

ostream& operator<<(ostream& out, const LexItem& tok){
    string *token = &tokenPrint[tok.GetToken()];
    
    out << *token;
    
    if(tok.GetToken() == SCONST || tok.GetToken() == RCONST || tok.GetToken() == ICONST || tok.GetToken() == IDENT || tok.GetToken() == ERR){
        out << " (" << tok.GetLexeme() << ")";
    }
    
    return out;
}

LexItem currentToken;
LexItem previousToken;

LexItem getNextToken(istream& in, int& linenum){
    enum TokenState { START, INID, INSTRING, ININT, INREAL, INCOMMENT, SIGN } lexstate = START;

    string lexeme;
    char character;
    
    while(in.get(character)){
        switch(lexstate){
            case START:
                if(character == '\n'){
                    linenum++;                    
                }
                if (in.peek() == -1) {
                    if (previousToken.GetToken() != END){
                        return LexItem(ERR, "No END Token", previousToken.GetLinenum());
                    }
                    return LexItem(DONE, lexeme, linenum);
                }
                if(isspace(character)){
                    continue;
                }
                
                // Check for comment
                lexeme = character;
                if(character == '/' && char(in.peek()) == '/'){
                    lexstate = INCOMMENT;
                    continue;
                }
                
                // Check for signs
                if(character == '+' || character == '-' || character == '*' || character == '/' || character == '(' || character == ')' || character == '=' || character == ',' || character == ';'){
                    lexstate = SIGN;
                    continue;
                }
                
                // Check for string
                if(character == '\"'){
                    lexstate = INSTRING;
                    continue;
                }
                
                // Check for integers
                if(isdigit(character)){
                    lexstate = ININT;
                    continue;
                }
                
                // Check for reals
                if(character == '.'){
                    lexstate = INREAL;
                    continue;
                }
                
                // Check for identifiers
                if(isalpha(character)){
                    lexstate = INID;
                    continue;
                }
                
                return LexItem(ERR, lexeme, linenum);
                
            case INID:
                if(regex_match(lexeme + character, regex("[a-zA-Z][a-zA-Z0-9]*"))){
                    lexeme += character;
                }
                if(in.peek() == -1 || !regex_match(lexeme + character, regex("[a-zA-Z][a-zA-Z0-9]*"))){
                    lexstate = START;
                    in.putback(character);
                    if(lexeme == "print"){
                        currentToken = LexItem(PRINT, lexeme, linenum);
                    }
                    else if (lexeme == "end") {
                        currentToken = LexItem(END, lexeme, linenum);
                    }
                    else if(lexeme == "if"){
                        currentToken = LexItem(IF, lexeme, linenum);
                    }
                    else if(lexeme == "then"){
                        currentToken = LexItem(THEN, lexeme, linenum);
                    }
                    else{
                        if(previousToken.GetToken() == IDENT){
                            return LexItem(ERR, lexeme, linenum);
                        }
                        currentToken = LexItem(IDENT, lexeme, linenum);
                    }

                    previousToken = currentToken;
                    return currentToken;
                }
                break;
        
            case INSTRING:
                if(previousToken == ERR){
                    return LexItem(ERR, "No Begin Token", linenum);
                }
                if(character == 10){
                    return LexItem(ERR, lexeme, linenum);
                }
                if(regex_match(lexeme + character, regex("\"[ -~]*"))){
                    if(character == '\\' && in.peek() == '\"'){
                        lexeme += character;
                        in.get(character);
                        lexeme += character;
                        continue;
                    }
                    else{
                        lexeme += character;
                    }
                }
                if(regex_match(lexeme + character, regex("\"[ -~]*\""))){
                    lexstate = START;
                    currentToken = LexItem(SCONST, lexeme, linenum);
                    previousToken = currentToken;
                    return currentToken;
                }
                break;

            case ININT:
                if(previousToken == ERR){
                    return LexItem(ERR, "No Begin Token", linenum);             
                }
                if(isalpha(character)){
                    return LexItem(ERR, lexeme + character, linenum);                
                }
                if(regex_match(lexeme + character, regex("[0-9]+"))){
                    lexeme += character;
                }
                else if(character == '.'){
                    lexstate = INREAL;
                    in.putback(character);
                    continue;
                }
                else{
                    lexstate = START;
                    in.putback(character);
                    currentToken = LexItem(ICONST, lexeme, linenum);
                    previousToken = currentToken;
                    return currentToken;
                }
                break;

            case INREAL:
                if(previousToken == ERR){
                    return LexItem(ERR, "No Begin Token", linenum);
                }
                if(isalpha(character)){
                    return LexItem(ERR, lexeme + character, linenum);
                }
                if(regex_match(lexeme + character, regex("[0-9]*\\.[0-9]+"))){
                    lexeme += character;
                }
                else if(regex_match(lexeme + character, regex("[0-9]*\\.[0-9]*"))){
                    lexeme += character;
                }
                else {
                    if(lexeme[lexeme.length() - 1] == '.'){
                        return LexItem(ERR, lexeme, linenum);
                    }
                    lexstate = START;
                    in.putback(character);
                    currentToken = LexItem(RCONST, lexeme, linenum);
                    previousToken = currentToken;
                    return currentToken;
                }
                break;

            case INCOMMENT:
                if(character == '\n'){
                    linenum++;
                    lexstate = START;
                }
                continue;

            case SIGN:
                if(previousToken == ERR){
                    return LexItem(ERR, "No Begin Token", linenum);
                }
                if(lexeme == "+" || lexeme == "*" || lexeme == "/"){
                    Token token = previousToken.GetToken();
                    if(token == IDENT || token == ICONST || token == RCONST){
                        lexstate = START;
                        in.putback(character);
                        if(lexeme == "+"){
                            currentToken = LexItem(PLUS, lexeme, linenum);
                        }
                        else if(lexeme == "*"){
                            currentToken = LexItem(MULT, lexeme, linenum);
                        }
                        else{
                            currentToken = LexItem(DIV, lexeme, linenum);
                        }
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                if(lexeme == "-"){
                    Token token = previousToken.GetToken();
                    if(token == IDENT || token == ICONST || token == RCONST){
                        lexstate = START;
                        in.putback(character);
                        currentToken = LexItem(MINUS, lexeme, linenum);
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                if(lexeme == "("){
                    Token token = previousToken.GetToken();
                    if(token == IF || token == PLUS || token == MINUS || token == MULT || token == DIV){
                        lexstate = START;
                        in.putback(character);
                        currentToken = LexItem(LPAREN, lexeme, linenum);
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                if(lexeme == ")"){
                    Token token = previousToken.GetToken();
                    if(token == ICONST || token == RCONST || token == IDENT){
                        lexstate = START;
                        in.putback(character);
                        currentToken = LexItem(RPAREN, lexeme, linenum);
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                if(lexeme == ","){
                    Token token = previousToken.GetToken();
                    if(token == SCONST){
                        lexstate = START;
                        in.putback(character);
                        currentToken = LexItem(COMA, lexeme, linenum);
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                if (lexeme == ":") {
                    Token token = previousToken.GetToken();
                    if(token == SCONST || token == ICONST || token == RCONST || token == IDENT){
                        lexstate = START;
                        in.putback(character);
                        currentToken = LexItem(COLON, lexeme, linenum);
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                if (lexeme == "=") {
                    Token token = previousToken.GetToken();
                    if(token == SCONST || token == ICONST || token == RCONST || token == IDENT){
                        lexstate = START;
                        in.putback(character);
                        currentToken = LexItem(EQUAL, lexeme, linenum);
                        previousToken = currentToken;
                        return currentToken;
                    }
                    else{
                        return LexItem(ERR, lexeme + character, linenum);
                    }
                }
                break;
        }
    }
    return LexItem(DONE, "", linenum);
}
