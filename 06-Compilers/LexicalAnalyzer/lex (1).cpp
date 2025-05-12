#include <iostream>

#include <regex>

#include <map>

#include "lex.h"

/**
 * Current token is used to set previous token before returned
 * Previous token is used to compare to the current token and check
 * for invalid arangment such as no begin token.
 */
LexItem currentToken;
LexItem previousToken;

ostream& operator<<(ostream& out, const LexItem& tok) {

bool eval = (tok.GetToken() == SCONST) || (tok.GetToken() == RCONST) ||
(tok.GetToken() == ICONST) || (tok.GetToken() == IDENT) ||
(tok.GetToken() == ERR);

if (eval)
std::cout << " " << tok.GetLexeme() << "";
return out;
}

/**
 * Return the next token within a file
 */
LexItem id_or_kw(const string& lexeme, int linenum)
{
    
    map<string,Token>::iterator iter=kwmap.find(lexeme);
    if(iter!=kwmap.end()){
        return LexItem(iter->second,lexeme,linenum);
    }
    return LexItem(IDENT,lexeme,linenum);
}

LexItem getNextToken(istream & in , int & linenum) {
  enum TokenState {
    START,
    INID,
    INSTRING,
    ININT,
    INREAL,
    INCOMMENT,
    SIGN
  }
  lexstate = START;
  std::string lexeme = "";
  char character;

  // Search until a token is found or eof is reached.
  while ( in .get(character)) {
    switch (lexstate) {
      // Basic state of searching for a token
    case START:
      if (character == '\n')
        linenum++;

      // If eof is found finish searching
      if ( in .peek() == -1) {
        if (previousToken.GetToken() != END)
         // return LexItem(ERR, "No END Token", previousToken.GetLinenum());
        return LexItem(DONE, lexeme, linenum);
      }

      // Spaces are meaningless for token analysis and are skipped
      if (std::isspace(character))
        continue;

      lexeme = character;

      // Check for comment with //
      if (character == '!') {
        lexstate = INCOMMENT;
        continue;
      }

      // Check for signs
      if (character == '+' || character == '-' || character == '*' ||
        character == '/' || character == '(' || character == ')' ||
        character == '=' || character == ',' || character == ';' || character == ':') {
        lexstate = SIGN;
        continue;
      }

      // Check for string
      if (character == '\"' || character == '\'') {
        lexstate = INSTRING;
        continue;
      }

      // Check for ints
      if (std::isdigit(character)) {
        lexstate = ININT;
        continue;
      }

      // Check for reals
      if (character == '.') {
        lexstate = INREAL;
        continue;
      }

      // Check for identifiers
      if (std::isalpha(character)) {
        lexstate = INID;
        continue;
      }

      // If a character cannot be classified into a state it must be an error
      return LexItem(ERR, lexeme, linenum);

    case INID:
      // Regex is used to match strings to proper formatting
      if (std::regex_match(lexeme + character, std::regex("[a-zA-Z][a-zA-Z0-9]*")))
        lexeme += character;
      if ( in .peek() == -1 || !std::regex_match(lexeme + character, std::regex("[a-zA-Z][a-zA-Z0-9]*"))) {
        lexstate = START; in .putback(character);
        currentToken = id_or_kw(lexeme, linenum);

        // Check for reserved keywords as identifiers
         /*
        if (lexeme == "PROGRAM") {
          if (previousToken.GetToken() != ERR)
            return LexItem(ERR, lexeme, linenum);
          currentToken = LexItem(PROGRAM, lexeme, linenum);
        } else if (lexeme == "Print")
          currentToken = LexItem(PRINT, lexeme, linenum);
        else if (lexeme == "END") {
          if (previousToken.GetToken() != COMA)
            return LexItem(ERR, previousToken.GetLexeme(), linenum);
          currentToken = LexItem(END, lexeme, linenum);
        } else if (lexeme == "IF")
          currentToken = LexItem(IF, lexeme, linenum);
        else if (lexeme == "THEN")
          currentToken = LexItem(THEN, lexeme, linenum);
        else if (lexeme == "INTEGER")
		  currentToken = LexItem(INTEGER, lexeme, linenum);
		else if (lexeme == "REAL")
		  currentToken = LexItem(REAL, lexeme, linenum);
		else if (lexeme == "CHAR")
		  currentToken = LexItem(CHAR, lexeme, linenum);
        else {
          if (previousToken.GetToken() == IDENT)
            return LexItem(ERR, lexeme, linenum);
          currentToken = LexItem(IDENT, lexeme, linenum);
         
        }*/
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case INSTRING:
      // String cannot contain multiple lines, must be an error
      if (character == 10)
        return LexItem(ERR, lexeme, linenum);

      // Check lexeme for unfished string
      if ((std::regex_match(lexeme + character, std::regex("\"[ -~]*"))) || (std::regex_match(lexeme + character, std::regex("\'[ -~]*")))) {
        if (character == '\\' && ((in.peek() == '\"')|| (in.peek() == '\''))) {
          lexeme += character; in .get(character);
          lexeme += character;
          continue;
        } else
          lexeme += character;
      }

      // Check lexeme for finished string
      if ((std::regex_match(lexeme + character, std::regex("\"[ -~]*\""))) || (std::regex_match(lexeme + character, std::regex("\'[ -~]*\'")))) {
        lexstate = START;
        currentToken = LexItem(SCONST, lexeme, linenum);
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case ININT:
      // Checks if an alpha character is next to an integer number
      if (std::isalpha(character))
        return LexItem(ERR, lexeme + character, linenum);
      if (std::regex_match(lexeme + character, std::regex("[0-9]+"))) {
        lexeme += character;
      }
      // If a period is found then the int is actual a real number
      else if (character == '.') {
        lexstate = INREAL; in .putback(character);
        continue;
      } else {
        lexstate = START; in .putback(character);
        currentToken = LexItem(ICONST, lexeme, linenum);
        previousToken = currentToken;
        return currentToken;
      }
      break;

    case INREAL:
      // Checks if an alpha character is next to a real number
      if (std::isalpha(character))
        return LexItem(ERR, lexeme + character, linenum);
      if (std::regex_match(lexeme + character, std::regex("[0-9]*\\.[0-9]+"))) {
        if(lexeme==".")
            lexeme="0"+lexeme;
        lexeme += character;
      } else if (std::regex_match(lexeme + character, std::regex("[0-9]*\\.[0-9]*"))) {
             if(lexeme==".")
            lexeme="0"+lexeme;
        lexeme += character;
      } else {
        if (lexeme[lexeme.length() - 1] == '.'){
            lexeme.pop_back();
          return LexItem(ERR, lexeme, linenum);}
        lexstate = START; in .putback(character);
        currentToken = LexItem(RCONST, lexeme, linenum);
        previousToken = currentToken;

        return currentToken;
      }
      break;

    case INCOMMENT:
      // Because comment is not a token it can be ignored
      if (character == '!') {
        linenum++;
        lexstate = START;
      }
      continue;

    case SIGN:
      /**
       * Signs are a little more complex and have to be handled individually
       * for the most part. Each sign can have a differnet type of token
       * in front of it requiring differnt checks.
       */
      if (lexeme == "+" || lexeme == "*" || lexeme == "/") {
        Token token = previousToken.GetToken();
        if (token == IDENT || token == ICONST || token == RCONST) {
          lexstate = START; in .putback(character);
          if (lexeme == "+")
            currentToken = LexItem(PLUS, lexeme, linenum);
          else if (lexeme == "*")
            currentToken = LexItem(MULT, lexeme, linenum);
          else
            currentToken = LexItem(DIV, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "-") {
        Token token = previousToken.GetToken();
        if (token == IDENT || token == ICONST || token == RCONST || token == EQUAL) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(MINUS, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "(") {
        Token token = previousToken.GetToken();
        if (token == IF || token == EQUAL || token == PLUS || token == MINUS ||
          token == MULT || token == DIV) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(LPAREN, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == ")") {
        Token token = previousToken.GetToken();
        if (token == ICONST || token == RCONST || token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(RPAREN, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "=") {
        Token token = previousToken.GetToken();
        if (token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(EQUAL, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == ",") {
        Token token = previousToken.GetToken();
        if (token == SCONST || token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(COMA, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == "::") {
        Token token = previousToken.GetToken();
        if (token == SCONST || token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(COLON, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      if (lexeme == ";") {
        Token token = previousToken.GetToken();
        if (token == SCONST || token == ICONST || token == RCONST || token == IDENT) {
          lexstate = START; in .putback(character);
          currentToken = LexItem(COMA, lexeme, linenum);
          previousToken = currentToken;
          return currentToken;
        } else
          return LexItem(ERR, lexeme + character, linenum);
      }
      break;
    }
  }

  return LexItem(DONE, "", linenum);
}
