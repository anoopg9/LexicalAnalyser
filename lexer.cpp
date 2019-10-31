#include <iostream>
#include "lex.h"
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <cmath>

using namespace std;

bool flag = true;
Lex getNextToken(istream &in, int &linenum)
{
    enum LexState
    {
        BEGIN,
        INID,
        stringState,
        intState,
        inComment
        /* others */
    };
    LexState lexstate = BEGIN;
    string lexeme;
    char ch;
    //cout<< ch << ":" << linenum<< endl;
    while (in.get(ch))
    {
        if (ch == '\n')
        {
            linenum++;
            //continue;
        }
        switch (lexstate)
        {
        case BEGIN:
            if (isspace(ch))
                continue;
            lexeme = ch;
            if (isalpha(ch))
            {
                lexstate = INID;

                continue;
            }
            else if (isdigit(ch))
            {
                lexstate = intState;

                continue;
            }
            else if (ch == '"')
            {
                lexeme = "";
                lexstate = stringState;
                // lexeme += ch;
                break;
            }

            //check for comments
            //check for slash
            else if (ch == '/')
            {
                //check for double slash
                if (ch == '/' && in.peek() == '/')
                {
                    lexstate = inComment;
                }
                else
                {
                    return Lex(SLASH, lexeme, linenum);
                }
            }
            //check for plus
            else if (ch == '+')
            {
                return Lex(PLUS, lexeme, linenum);
            }
            //check for minus
            else if (ch == '-')
            {
                return Lex(MINUS, lexeme, linenum);
            }
            //check for multiplication
            else if (ch == '*')
            {
                return Lex(STAR, lexeme, linenum);
            }
            //check for NOT
            else if (ch == '!')
            {
                return Lex(BANG, lexeme, linenum);
            }
            //check for left parenthesis
            else if (ch == '(')
            {
                return Lex(LPAREN, lexeme, linenum);
            }
            //check for right parenthesis
            else if (ch == ')')
            {
                return Lex(RPAREN, lexeme, linenum);
            }
            //check for semicolon and return its identifier
            else if (ch == ';')
            {
                return Lex(SC, lexeme, linenum);
            }
            else
            {
                return Lex(ERR, lexeme, linenum);
            }
            break;
            // continue;
        //look for string case if present
        case stringState:

            // check the error
            if (ch == '\\')
            {
                ch = in.get();
                if (ch == 'n')
                {
                    lexeme += '\n';
                    continue;
                }
                else
                {
                    lexeme += ch;
                    continue;
                }
            }
            if (ch == '\n')
            {

                flag = false;
                lexeme += ch;
                linenum--;
                continue;
            }

            //check for " and return error if present
            else if (ch == '"')
            {
                if (flag)
                {
                    return Lex(STR, lexeme, linenum);
                }
                else
                {
                    lexeme = "\"" + lexeme;
                    return Lex(ERR, lexeme, linenum);
                }

                continue;
            }
            else
            {
                lexeme += ch;
                continue;
            }
            //break the case
            break;

            //check for identifier case
        case INID:
            if (isalpha(ch) || isdigit(ch))
            {
                lexeme += ch;
                continue;
            }
            if (lexeme == "let")
            {
                return Lex(LET, lexeme, linenum);
            }

            else
            {
                if (ch == '\n')
                {
                    linenum--;
                }
                in.putback(ch);
                return Lex(ID, lexeme, linenum);
            }

            break;

        //check for integer state
        case intState:
            if (isdigit(ch))
            {
                lexeme += ch;
            }
            else if (isspace(ch))
            {
                return Lex(INT, lexeme, linenum);
            }
            else if (isalpha(ch))
            {
                lexeme += ch;
                return Lex(ERR, lexeme, linenum);
            }
            else
            {
                in.putback(ch);
                return Lex(INT, lexeme, linenum);
            }
            break;

        //check for comment and ignores it
        case inComment:
            if (ch != '\n')
            {
                continue;
            }
            else
            {
                lexstate = BEGIN;
            }
            break;
        }
    }

    //check if the file ended
    if (in.eof())
    {
        return Lex(DONE, "", linenum);
    }
    return Lex(ERR, lexeme, linenum);
}


//operator to print out
ostream &operator<<(ostream &out, const Lex &o)
{ // format output any way you like, and
    // send it to “out”
    if (o.GetToken() == STR)
    {
        out << "STR(" << o.GetLexeme() << ")";
        return out;
    }
    else if (o.GetToken() == INT)
    {
        out << "INT(" << o.GetLexeme() << ")";
        return out;
    }
    else if (o.GetToken() == ID)
    {
        out << "ID(" << o.GetLexeme() << ")";
        return out;
    }
    else if (o.GetToken() == PRINT)
    {
        out << "PRINT";
        return out;
    }
    else if (o.GetToken() == LET)
    {
        out << "LET";
        return out;
    }
    else if (o.GetToken() == IF)
    {
        out << "IF";
        return out;
    }
    else if (o.GetToken() == END)
    {
        out << "END";
        return out;
    }
    else if (o.GetToken() == BEGIN)
    {
        out << "BEGIN";
        return out;
    }
    else if (o.GetToken() == LOOP)
    {
        out << "LOOP";
        return out;
    }
    else if (o.GetToken() == PLUS)
    {
        out << "PLUS";
        return out;
    }
    else if (o.GetToken() == MINUS)
    {
        out << "MINUS";
        return out;
    }
    else if (o.GetToken() == SLASH)
    {
        out << "SLASH";
        return out;
    }
    else if (o.GetToken() == STAR)
    {
        out << "STAR";
        return out;
    }
    else if (o.GetToken() == BANG)
    {
        out << "BANG";
        return out;
    }
    else if (o.GetToken() == LPAREN)
    {
        out << "LPAREN";
        return out;
    }
    else if (o.GetToken() == RPAREN)
    {
        out << "RPAREN";
        return out;
    }
    else if (o.GetToken() == SC)
    {
        out << "SC";
        return out;
    }
    else
    {
        out << "Error on line " << o.GetLinenum() + 1 << " (" << o.GetLexeme() << ")";
        return out;
    }
}