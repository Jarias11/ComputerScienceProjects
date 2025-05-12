/* 05/02/2021
 * Milton Flores
 * Programming Assignment 3
 * Spring 2021
*/
#include<cmath>

#include<sstream>
#include "parserInt.h"
static int error_count = 0;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults;
queue <Value> * ValQue;

Value v = 0;
namespace Parser {
    bool pushed_back = false;
    LexItem	pushed_token;

    static LexItem GetNextToken(istream& in, int& line) {
        if( pushed_back ) {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(LexItem & t) {
        if( pushed_back ) {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }

}

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;

}



//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
    bool dl = false, sl = false;
    LexItem tok = Parser::GetNextToken(in, line);


    if (tok.GetToken() == PROGRAM) {
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == IDENT) {
            SymTable[tok.GetLexeme()] = IDENT;
            dl = Decl(in, line);
            if( !dl  )
            {
                ParseError(line, "Incorrect Declaration in Program");
                return false;
            }
            sl = Stmt(in, line);
            if( !sl  )
            {
                ParseError(line, "Incorrect Statement in program");
                return false;
            }
            tok = Parser::GetNextToken(in, line);

            if (tok.GetToken() == END) {
                tok = Parser::GetNextToken(in, line);

                if (tok.GetToken() == PROGRAM) {
                    tok = Parser::GetNextToken(in, line);

                    if (tok.GetToken() == IDENT) 
                        if(SymTable[tok.GetLexeme()]==IDENT)
                        return true;
                    
                    else
                    {
                        ParseError(line, "Incorrect Program Name");
                        return false;
                    }
                }
                else
                {
                    ParseError(line, "Missing PROGRAM at the End");
                    return false;
                }
            }
            else
            {
                ParseError(line, "Missing END of Program");
                return false;
            }
        }
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }

    return true;
}

//Decl = Type : VarList
//Type = INTEGER | REAL | CHAR
bool Decl(istream& in, int& line) {
//    cout << "In Decl" << endl;
    bool status = false;
    LexItem tok;

    LexItem t = Parser::GetNextToken(in, line);


    if(t == INTEGER || t == REAL || t == CHAR) {
        tok = t;
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == COLON) {
            status = IdList(in, line, t);
            //cout<< tok.GetLexeme() << " " << (status? 1: 0) << endl;
            if (status)
            {
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

//Stmt is either a PrintStmt, ReadStmt, IfStmt, or an AssigStmt
//Stmt = AssigStmt | IfStmt | PrintStmt | ReadStmt
bool Stmt(istream& in, int& line) {
    bool status;
//    cout << "IN STMT" << endl;

    LexItem t = Parser::GetNextToken(in, line);

    switch( t.GetToken() ) {

        case PRINT:
            status = PrintStmt(in, line);

            if(status)
                status = Stmt(in, line);
            break;

        case IF:
            status = IfStmt(in, line);
            if(status)
                status = Stmt(in, line);
            break;

        case IDENT:
            Parser::PushBackToken(t);
            status = AssignStmt(in, line);
            if(status)
                status = Stmt(in, line);
            break;

        case READ:
            status = ReadStmt(in, line);

            if(status)
                status = Stmt(in, line);
            break;

        default:
            Parser::PushBackToken(t);
            return true;
    }

    return status;
}

//PrintStmt:= print, ExpreList
bool PrintStmt(istream& in, int& line) {
    LexItem t;
    ValQue = new queue<Value>;

    if ( (t=Parser::GetNextToken(in,line)) != COMA ) {
        ParseError(line, "Missing a Comma");
        return false;
    }
    bool ex = ExprList(in, line);

    if( !ex ) {
        ParseError(line, "Missing expression after print");
        while(!(*ValQue).empty()){
            ValQue->pop();
        }
        delete ValQue;
        return false;
    }
    while (!(*ValQue).empty()){
        Value nextVal = (*ValQue).front();
        cout << nextVal;
        ValQue->pop();
    }
    cout<< endl;
    return ex;
}

//IfStmt:= if (Expr) then {Stmt} END IF
bool IfStmt(istream& in, int& line) {
    bool ex=false ;
    LexItem t;

    if( (t=Parser::GetNextToken(in, line)) != LPAREN ) {

        ParseError(line, "Missing Left Parenthesis");
        return false;
    }

    ex = LogicExpr(in, line, v);
    if( !ex ) {
        ParseError(line, "Missing if statement Logic Expression");
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
        ParseError(line, "Missing statement for IF");
        return false;
    }else{
    while(t != END){
    t = Parser::GetNextToken(in,line);    
    }
    Parser::PushBackToken(t);
    }



    if((t=Parser::GetNextToken(in, line)) != END ) {

        ParseError(line, "Missing END of IF");
        return false;
    }
    if((t=Parser::GetNextToken(in, line)) != IF ) {

        ParseError(line, "Missing IF at End of IF statement");
        return false;
    }

    return true;
}

bool ReadStmt(istream& in, int& line)
{

    LexItem t;

    if( (t=Parser::GetNextToken(in, line)) != COMA ) {

        ParseError(line, "Missing a Comma");
        return false;
    }

    bool ex = VarList(in, line);

    if( !ex ) {
        ParseError(line, "Missing Variable after Read Statement");
        return false;
    }



    return ex;
}
//IdList:= IDENT {,IDENT}
bool IdList(istream& in, int& line, LexItem & tok) {
//    cout << "IN IDLIST" << endl;

    LexItem t = Parser::GetNextToken(in, line);
    SymTable.insert(pair<string, Token>(t.GetLexeme(), tok.GetToken()));

    if ((t = Parser::GetNextToken(in, line)) == COMA) {
        IdList(in, line, t);
    }
    else {
        Parser::PushBackToken(t);
        return true;
    }

    return true;
}

//VarList
bool VarList(istream& in, int& line)
{
    bool status = false;
    string identstr;
    LexItem t;

    status = Var(in, line, t);

    if(!status)
    {
        ParseError(line, "Missing Variable");
        return false;
    }

    LexItem tok = Parser::GetNextToken(in, line);

    if (tok == COMA) {
        status = VarList(in, line);
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

//Var:= ident
bool Var(istream& in, int& line, LexItem & tok)
{
    //called only from the AssignStmt function
    string identstr;



    if (tok == IDENT){
        identstr = tok.GetLexeme();
        if (!(SymTable.find(identstr)->second))
        {
            ParseError(line, "Undeclared Variable");
            return false;

        }
        return true;
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    return false;
}

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
//    cout << "In assignstmt" << endl;
    Value val1;
    Value result;

    bool varstatus = false, status = false;
    LexItem t = Parser::GetNextToken(in, line);
    LexItem type = t;
//    cout << t << endl;

    varstatus = Var(in, line, t);


    if (varstatus){
        t = Parser::GetNextToken(in, line);

        if (t == ASSOP){
            status = Expr(in, line,val1);
            if(!status) {
                ParseError(line, "Missing Expression in Assignment Statment");
                return status;
            }
            result = val1;

        }
        else if(t.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }
        else {
            ParseError(line, "Missing Assignment Operator =");
            return false;
        }
    }
    else {
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }

//    cout <<  type  << " Variable name" << endl;
//    cout << result  << " Variable value"<< endl;

    TempsResults.insert(pair<string,Value>(type.GetLexeme(), result));
    defVar.insert(pair<string,bool>(type.GetLexeme(), true));
    return status;
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
    bool status = false;
    Value val1;

    status = Expr(in, line,val1);
    if(!status){
        ParseError(line, "Missing Expression");
        return false;
    }
    ValQue->push(val1);

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

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value & retVal) {
//    cout << "In Expr" << endl;
    Value val1, val2;
    bool t1 = Term(in, line,val1);
    LexItem tok;

    if( !t1 ) {
        return false;
    }

    retVal = val1;

    tok = Parser::GetNextToken(in, line);
    if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while ( tok == PLUS || tok == MINUS )
    {
        t1 = Term(in, line, val2);
        if( !t1 )
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        if(retVal.GetType() == VCHAR || val2.GetType() == VCHAR) {
            ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
            return false;
        }
        else{
            if(tok == PLUS){
                retVal = retVal + val2;
            }
            else if(tok == MINUS) {
                retVal = retVal - val2;
            }
        }

        tok = Parser::GetNextToken(in, line);
        if(tok.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }


    }
    Parser::PushBackToken(tok);
    return true;
}

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value &retVal) {
//    cout << "in Term" << endl;
    Value val1, val2;
    bool t1 = SFactor(in, line,val1);
    LexItem tok;

    if( !t1 ) {
        return false;
    }

    retVal = val1;

    tok = Parser::GetNextToken(in, line);
    if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while ( tok == MULT || tok == DIV )
    {
        t1 = SFactor(in, line, val2);
        if( !t1 )
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        if(retVal.GetType() == VCHAR || val2.GetType() == VCHAR) {
            ParseError(line, "Run-Time Error-Illegal Mixed Type Operands");
            return false;
        }
        else{
            if(tok == MULT){
                retVal = retVal * val2;
            }
            else if(tok == DIV) {
                if((val2.GetType() == VINT && val2.GetInt() == 0) || (val2.GetType() == VREAL && val2.GetReal() == 0)){
                ParseError(line, "Run-Time Error-Illegal Division by Zero");
                    return false;
                
                }
                retVal = retVal / val2;
            }
        }

        tok = Parser::GetNextToken(in, line);
        if(tok.GetToken() == ERR){
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }


    }
    Parser::PushBackToken(tok);
    return true;
}

//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line, Value &retVal)
{
    Value val1;
//    cout << "In Sfactor" << endl;
    LexItem t = Parser::GetNextToken(in, line);

    int sign = 0;
    if(t == MINUS )
    {
        sign = -1;
    }
    else if(t == PLUS)
    {
        sign = 1;
    }
    else
        Parser::PushBackToken(t);

    Factor(in, line, sign,val1);
    retVal = val1;
//    cout << retVal << " retval out of factor" << endl;
    return true;
}
//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value & retVal)
{
    bool t1 = Expr(in, line, retVal);
    LexItem tok;
    Value val1, val2;

    if( !t1 ) {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    if ( tok == LTHAN  || tok == EQUAL)
    {
        t1 = Expr(in, line,retVal);
        if( !t1 )
        {
            ParseError(line, "Missing expression after relational operator");
            return false;
        }
        return true;
    }
    Parser::PushBackToken(tok);
    return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value &retVal) {
//    cout << "in Factor" << endl;

    LexItem tok = Parser::GetNextToken(in, line);
//    cout << tok << " :In factor" << endl;


    if( tok == IDENT ) {
        //check if the var is defined
        string lexeme = tok.GetLexeme();
        retVal = TempsResults.find(tok.GetLexeme())->second;
        if (!(defVar.find(lexeme)->second))
        {
            ParseError(line, "Undefined Variable");
            return false;
        }

        retVal = TempsResults.find(tok.GetLexeme())->second;
//        cout << retVal << " Coming out of factor" << endl;
        return true;
    }
    else if( tok == ICONST ) {
        retVal.SetType(VINT);
        if (sign == -1){
            retVal.SetInt(stoi(tok.GetLexeme()) * sign);
            return true;
        }
        stringstream g(tok.GetLexeme());
        int temp = 0;
        g>>temp;
        retVal.SetInt(temp);
        return true;
    }
    else if( tok == SCONST ) {
        retVal = tok.GetLexeme();
        return true;
    }
    else if( tok == RCONST ) {
        retVal.SetType(VREAL);
        retVal.SetReal(stof(tok.GetLexeme()));
        return true;
    }
    else if( tok == LPAREN ) {
        bool ex = Expr(in, line,retVal);
        if( !ex ) {
            ParseError(line, "Missing expression after (");
            return false;
        }
        if( Parser::GetNextToken(in, line) == RPAREN )
            return ex;

        ParseError(line, "Missing ) after expression");
        return false;
    }
    else if(tok.GetToken() == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }

    ParseError(line, "Unrecognized input");
    return 0;
}
