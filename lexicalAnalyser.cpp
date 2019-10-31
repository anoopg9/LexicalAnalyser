#include <iostream>
#include "lex.h"
#include <fstream>
#include <string.h>
#include <set>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

ifstream inFile;
istream *in = &cin;
int lineNumber = 0;

bool flagV = false;
bool flagIds = false;
bool flagConsts = false;

int main(int argc, char *argv[])
{

    //run a loop to keep a count
    for (int i = 1; i < argc; i++)
    {
        string testFlag = argv[i];
        //check the for the flag argument
        //check for minusV
        if (testFlag == "-v")
        {
            flagV = true;
        }
        //check for minusIDS
        else if (testFlag == "-ids")
        {
            flagIds = true;
        }
        //check for minusConsts
        else if (testFlag == "-consts")
        {
            flagConsts = true;
        }
        // check if file is present
        else if (testFlag[0] != '-')
        {
            //open the file
            inFile.open(testFlag);
            if (inFile.fail())
            {
                cout << "CANNOT OPEN " << testFlag << endl;
                exit(0);
            }
            /* //if file is empty run this if statement this runs emptyfile runcase
            if (inFile.peek() == std::ifstream::traits_type::eof())
            {
                cout << "Lines: " << lineNumber << endl;
                exit(0);
            }
               */
            //if one file is already open then the other file should print this
            if ((inFile.is_open()) && i != (argc - 1))
            {
                cout << "ONLY ONE FILE NAME ALLOWED" << endl;
                exit(0);
            }
            //make you in point to reference of the file
            in = &inFile;
            inFile.close();
        }

        // if not then print unrecognized flag
        else
        {
            cout << "UNRECOGNIZED FLAG " << testFlag << endl;
            exit(0);
        }
    }
    //check for the argument and files
    inFile.open(argv[argc - 1]);

    bool nStrings = false;
    //numbers of tokens
    int nTokens = 0;

    bool nInts = false;
    //create a vector to store the identifers
    vector<string> vecIds;
    //create a vector to store the strings
    vector<string> vecStrings;
    //create a vector to store the ints
    vector<int> vecInts;
    Lex tok;
    while ((tok = getNextToken(*in, lineNumber)).GetToken() != DONE && tok != ERR)
    {
        //Run minusV and print the Token
        if (flagV)
        {
            cout << tok << endl;
        }
        //check for minus IDS
        if (flagIds && tok.GetToken() == ID)
        {
            //push that ID to vector
            vecIds.push_back(tok.GetLexeme());
        }
        //check minus Consts
        if (flagConsts && tok.GetToken() == STR)
        {
            vecStrings.push_back(tok.GetLexeme());
            nStrings = true;
        }
        if (flagConsts && tok.GetToken() == INT)
        {
            vecInts.push_back(stoi(tok.GetLexeme()));
            nInts = true;
        }

        nTokens++;
    }
    if (flagIds)
    {
        // intialize vector
        vector<string>::iterator it;
        //Sort the Vector and run a loop
        sort(vecIds.begin(), vecIds.end());
        it = vecIds.begin();
        auto copy = unique(vecIds.begin(), vecIds.end());
        vecIds.erase(copy, vecIds.end());
        if (it != vecIds.end())
        {
            cout << "IDENTIFIERS: " << *it++;
            //run a loop through vector to add comma
            for (; it != vecIds.end(); it++)
            {
                cout << ", " << *it;
            }
            // just end after the loop
            cout << endl;
        }
    }
    if (nStrings)
    {
        sort(vecStrings.begin(), vecStrings.end());
        vector<string>::iterator str;

        str = vecStrings.begin();
        if (str != vecStrings.end())
        {
            cout << "STRINGS:" << endl;
            //run a loop through vector to add comma
            for (; str != vecStrings.end(); str++)
            {
                cout << *str << endl;
            }
        }
    }
    if (nInts)
    {

        sort(vecInts.begin(), vecInts.end());
        vector<int>::iterator nInt;

        nInt = vecInts.begin();
        if (nInt != vecInts.end() && tok != ERR)
        {
            cout << "INTEGERS:" << endl;
            //run a loop through vector to add comma
            for (; nInt != vecInts.end(); nInt++)
            {
                cout << *nInt << endl;
            }
        }
    }

    // if Tok is not present print out the error
    if (tok == ERR)
    {

        cout << tok << endl;
        return -1;
    }
    //print out the Lines Tokens
    else
    {
        cout << "Lines: " << tok.GetLinenum() << endl;
        if (tok.GetLinenum() != 0)
        {
            cout << "Tokens: " << nTokens << endl;
        }
    }

    return 0;
}
