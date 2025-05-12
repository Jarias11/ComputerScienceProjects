/*
 * parse.cpp
 *
 *  Created on: Apr 14, 2021
 *      Author: Jarias
 */
#include <iostream>

using namespace std;

#include "parse.h"

using std::map;
map<string, bool> defVar;

namespace Parser {
   bool pushed_back = false;
   LexItem   pushed_token;

   static LexItem GetNextToken(istream& in, int& line) {
       if( pushed_back ) {
           pushed_back = false;
           //cout<< "returned token: " << pushed_token.GetLexeme() << endl;
           return pushed_token;
       }
       return getNextToken(in, line);
   }

   static void PushBackToken(LexItem & t) {
       if( pushed_back ) {
           //cout<< "Pushed token abort: " << t.GetLexeme() << endl;
           abort();
       }
       pushed_back = true;
       pushed_token = t;
       //cout<< "Pushed token: " << t.GetLexeme() << endl;  
   }

}

static int error_count = 0;

void ParseError(int line, string msg)
{
   ++error_count;
   cout << line << ": " << msg << endl;
}



//Program is: Prog := begin StmtList end
bool Prog(istream& in, int& line)
{
   bool sl = false;
   LexItem tok = Parser::GetNextToken(in, line);
   cout << "in Prog" << endl;
  
   if (tok.GetToken() == PROGRAM) {
       sl = Decl(in, line);
       if( !sl )
           ParseError(line, "No statements in program");
       if( error_count > 0 )
           return false;
   }
   else if(tok.GetToken() == ERR){
       ParseError(line, "Unrecognized Input Pattern");
       cout << "(" << tok.GetLexeme() << ")" << endl;
       return false;
   }
  
   tok = Parser::GetNextToken(in, line);
  
   if (tok.GetToken() == END)
       return true;
   else if(tok.GetToken() == ERR){
       ParseError(line, "Unrecognized Input Pattern");
       cout << "(" << tok.GetLexeme() << ")" << endl;
       return false;
   }
   else
       cout<<"Program not doin anything"<<endl;
       return false;
}

// StmtList is a Stmt followed by semicolon followed by a StmtList
bool Decl(istream& in, int& line) {
	    // Decl = (INTEGER | REAL | CHAR) : IdList
	    bool status = false;
	    LexItem t = Parser::GetNextToken(in, line);
	    LexItem tok;
	    if(t == INTEGER || t == REAL || t == CHAR){
	    	tok = t;
	        tok = Parser::GetNextToken(in, line);
	        if(tok.GetToken() == COLON){
	            status = IdList(in, line, t);
	            if(status){
	                status = Decl(in, line);
	                return status;
	            }
	        }
	        else{
	            ParseError(line, "Missing Colon");
	            return false;
	        }
	    }
	    Parser::PushBackToken(t);
	    return true;
	}

//Stmt is either a PrintStmt, IfStmt, or an AssigStmt
bool Stmt(istream& in, int& line) {
   bool status;
   cout << "in Stmt" << endl;
   LexItem t = Parser::GetNextToken(in, line);
  
   switch( t.GetToken() ) {

   case PRINT:
       status = PrintStmt(in, line);
       if(status)
    	   status = Stmt(in,line);
       cout << "status: " << (status? true:false) <<endl;
       break;

   case IF:
       status = IfStmt(in, line);
       if(status)
          	   status = Stmt(in,line);
       break;

   case IDENT:
Parser::PushBackToken(t);
       status = AssignStmt(in, line);
       break;

   case END:
       Parser::PushBackToken(t);
       return true;
   case ERR:
       ParseError(line, "Unrecognized Input Pattern");
       cout << "(" << t.GetLexeme() << ")" << endl;
       return false;
   case DONE:
       return false;

   default:
       ParseError(line, "Invalid statement");
       return false;
   }

   return status;
}

//PrintStmt:= print ExpreList
bool PrintStmt(istream& in, int& line) {
    LexItem t;
   cout << "in PrintStmt" << endl;
   if((t=Parser::GetNextToken(in,line))!= COMA){
	   ParseError(line,"Missing Coma");
	   return false;
   }
   bool ex = ExprList(in, line);
  
   if( !ex ) {
       ParseError(line, "Missing expression after print");
       return false;
   }
  
   //Evaluate: print out the list of expressions values

   return ex;
}

//
bool ReadStmt(istream& in, int& line){
    // ReadStmt = READ , VarList

    LexItem tok;

    if((tok = Parser::GetNextToken(in, line)) != COMA){
        ParseError(line, "Missing a Comma");
        return false;
    }
    bool read = VarList(in, line);
    if(!read){
        ParseError(line, "Missing expression after print");
        return false;
    }
    return read;
}

//IfStmt:= if (Expr) then Stmt
bool IfStmt(istream& in, int& line) {
   bool ex=false ;
   LexItem t;
   cout << "in IfStmt" << endl;
   if( (t=Parser::GetNextToken(in, line)) != LPAREN ) {
      
       ParseError(line, "Missing Left Parenthesis");
       return false;
   }
  
   ex = Expr(in, line);
   if( !ex ) {
       ParseError(line, "Missing if statement expression");
       return false;
   }

   if((t=Parser::GetNextToken(in, line)) != RPAREN ) {
      
       ParseError(line, "Missing Right Parenthesis");
       return false;
   }
  
   if((t=Parser::GetNextToken(in, line)) != THEN ) {
      
       ParseError(line, "Missing THEN");
       return false;
   }

   bool st = Stmt(in, line);
   if( !st ) {
       ParseError(line, "Missing statement for if");
       return false;
   }
  
   //Evaluate: execute the if statement
  
   return st;
}

//
bool IdList(istream& in, int& line, LexItem tok){
    // IdList = IDENT {,IDENT}
    tok = Parser::GetNextToken(in, line);

    if(tok != IDENT){
        ParseError(line, "Error");
        return false;
    }
    else{
        return true;
    }
}

bool VarList(istream& in, int& line){
    // VarList = Var {,Var}

    cout << "in VarList" << endl;

    bool status = Var(in, line);

    if(!status){
        ParseError(line, "Error");
        return false;
    }

    LexItem tok = Parser::GetNextToken(in, line);
    cout << tok.GetLexeme() << endl;
    if(tok == COMA){
        status = VarList(in, line);
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Error");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else{
        Parser::PushBackToken(tok);
        return true;
    }

    return status;
}

//Var:= ident
bool Var(istream& in, int& line)
{
   //called only from the AssignStmt function
   string identstr;
   cout << "in Var" << endl;
   LexItem atok = Parser::GetNextToken(in, line);
  
   if (atok == IDENT){
       identstr = atok.GetLexeme();
       if (!(defVar.find(identstr)->second))
           defVar[identstr] = true;
       return true;
   }
   else if(atok.GetToken() == ERR){
       ParseError(line, "Unrecognized Input Pattern");
       cout << "(" << atok.GetLexeme() << ")" << endl;
       return false;
   }
   return false;
}

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
     bool varstatus = false;
    bool status = false;

    LexItem tok;

    varstatus = Var(in, line);
    cout << "varstatus:" << varstatus << endl;

    if(varstatus){
        if((tok = Parser::GetNextToken(in, line)) != EQUAL){
            status = Expr(in, line);
            if(!status){
                ParseError(line, "Error");
                return status;
            }
        }
        else if(tok.GetToken() == ERR){
            ParseError(line, "Error");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
        else{
            ParseError(line, "Error");
            return false;
        }
    }
    else{
        ParseError(line, "Error");
        return false;
    }
    return status;
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
   bool status = false;
   cout << "in ExprList" << endl;
   status = Expr(in, line);
   if(!status){
       ParseError(line, "Missing Expression");
       return false;
   }
  
   LexItem tok = Parser::GetNextToken(in, line);
  
   if (tok == COMA) {
       status = ExprList(in, line);
   }
   else if(tok.GetToken() == ERR){
       ParseError(line, "Unrecognized Input Pattern");
       cout << "(" << tok.GetLexeme() << ")" << endl;
       return false;
   }
   else{
       Parser::PushBackToken(tok);
       return true;
   }
   return status;
}

bool LogicExpr(istream& in, int& line){
    // LogicExpr = Expr (== | <) Expr

    bool status = Expr(in, line);
    if(!status){
        ParseError(line, "Expression missing");
        return false;
    }

    LexItem tok = Parser::GetNextToken(in, line);

    if(tok == LPAREN){
        tok = Parser::GetNextToken(in, line);
        if(tok == ASSOP || tok == LTHAN){
            ParseError(line, "No left parenthesis");
            return false;
        }
        if(Parser::GetNextToken(in, line) == RPAREN){
            ParseError(line, "No right parenthesis");
            return false;
        }
    }

    status = Expr(in, line);
    if(!status){
        ParseError(line, "Error");
        return false;
    }
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line) {
   bool t1 = Term(in, line);
   LexItem tok;
   cout << "in Expr" << endl;
   if( !t1 ) {
       return false;
   }
  
   tok = Parser::GetNextToken(in, line);
   if(tok.GetToken() == ERR){
       ParseError(line, "Unrecognized Input Pattern");
       cout << "(" << tok.GetLexeme() << ")" << endl;
       return false;
   }
   while ( tok == PLUS || tok == MINUS )
   {
       t1 = Term(in, line);
       if( !t1 )
       {
           ParseError(line, "Missing expression after operator");
           return false;
       }
      
       tok = Parser::GetNextToken(in, line);
       if(tok.GetToken() == ERR){
           ParseError(line, "Unrecognized Input Pattern");
           cout << "(" << tok.GetLexeme() << ")" << endl;
           return false;
       }      
      
       //Evaluate: evaluate the expression for addition or subtraction
   }
   Parser::PushBackToken(tok);
   return true;
}

//Term:= Factor {(*|/) Factor}
bool Term(istream& in, int& line) {
   cout << "in Term" << endl;
   bool t1 = SFactor(in, line);
   LexItem tok;
   cout << "status of factor1: " << t1<< endl;
   if( !t1 ) {
       return false;
   }
  
   tok   = Parser::GetNextToken(in, line);
   if(tok.GetToken() == ERR){
           ParseError(line, "Unrecognized Input Pattern");
           cout << "(" << tok.GetLexeme() << ")" << endl;
           return false;
   }
   while ( tok == MULT || tok == DIV )
   {
       t1 = SFactor(in, line);
       cout << "status of factor2: " << t1<< endl;
       if( !t1 ) {
           ParseError(line, "Missing expression after operator");
           return false;
       }
      
       tok   = Parser::GetNextToken(in, line);
       if(tok.GetToken() == ERR){
           ParseError(line, "Unrecognized Input Pattern");
           cout << "(" << tok.GetLexeme() << ")" << endl;
           return false;
       }
       //Evaluate: evaluate the expression for multiplication or division
   }
   Parser::PushBackToken(tok);
   return true;
}

bool SFactor(istream& in, int& line){
    // SFactor = Sign Factor | Factor

    LexItem tok = Parser::GetNextToken(in, line);
    if(tok == PLUS)
    	return Factor(in,  line, 1);
    if(tok == MINUS)
    	return Factor(in, line, -1);
    else{
    	Parser::PushBackToken(tok);
    	return Factor(in, line, 0);
    }
    
    
    
    
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign) {
   // Factor = IDENT| ICONST| RCONST| SCONST| (Expr)
    cout << "in Factor" << endl;

    LexItem tok = Parser::GetNextToken(in, line);

    if(tok == IDENT){
        // int val;
        string lexeme = tok.GetLexeme();
        if(!(defVar.find(lexeme) -> second)){
            ParseError(line, "Identity error");
            return false;
        }
        return true;
    }
    else if(tok == ICONST){
        return true;
    }
    else if(tok == RCONST){
        return true;
    }
    else if(tok == SCONST){
        return true;
    }

    else if(tok == LPAREN){
        bool ex = Expr(in, line);
        if(!ex){
            ParseError(line, "Left parenthesis");
            return false;
        }
        if(Parser::GetNextToken(in, line) == RPAREN){
            return ex;
        }

        ParseError(line, "Error");
        return false;
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Error");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }

    ParseError(line, "Error");
    return 0;
}

int ErrCount(){
    return error_count;
}