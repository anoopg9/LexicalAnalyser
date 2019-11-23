/*
 * main.cpp
 *
 * CS280 - Fall 2019
 * Lexical Analyzer test engine
 */

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include "lex.h"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int
main(int argc, char *argv[])
{
	int lineNumber = 0;

	Lex	tok;

	istream *in = &cin;
	ifstream file;

	bool vflag = false;
	bool idsflag = false;
	bool cflag = false;

	int tokCount = 0;
	map<string,bool> identifiers;
	map<string,bool> strings;
	map<int,bool> ints;

	for( int i=1; i<argc; i++ ) {
		string arg( argv[i] );
		if( arg == "-v" )
			vflag = true;
		else if( arg == "-ids" )
			idsflag = true;
		else if( arg == "-consts" )
			cflag = true;
		else if( arg[0] == '-' ) {
			cerr << "UNRECOGNIZED FLAG " << arg << endl;
			return 0;
		}
		else if( in != &cin ) {
			cerr << "ONLY ONE FILE NAME ALLOWED" << endl;
			return 0;
		}
		else {
			file.open(arg);
			if( file.is_open() == false ) {
				cerr << "CANNOT OPEN " << arg << endl;
				return 0;
			}

			in = &file;
		}
	}

	while( (tok = getNextToken(*in, lineNumber)) != DONE && tok != ERR ) {
		if( vflag )
			cout << tok << endl;

		++tokCount;
		if( tok == STR ) {
			strings[ tok.GetLexeme() ] = true;
		}
		else if( tok == INT ) {
			ints[ stoi(tok.GetLexeme()) ] = true;
		}
		else if( tok == ID ) {
			identifiers[ tok.GetLexeme() ] = true;
		}
	}

	if( tok == ERR ) {
		cout << "Error on line " << tok.GetLinenum()+1 << " (" << tok.GetLexeme() << ")" << endl;
		return 0;
	}

	if( cflag ) {
        if( strings.size() > 0 ) {
		    cout << "STRINGS:" << endl;
            for( const auto& sp : strings )
                cout << sp.first << endl;
        }
        if( ints.size() > 0 ) {
		    cout << "INTEGERS:" << endl;
            for( const auto& sp : ints )
                cout << sp.first << endl;
        }
	}

	if( idsflag && identifiers.size() > 0 ) {
     	cout << "IDENTIFIERS: ";
     	auto it = identifiers.begin();
     	cout << it->first;

		for( it++; it != identifiers.end(); it++ )
			cout << ", " << it->first;
		cout << endl;
	}
    
    // summary
	cout << "Lines: " << lineNumber << endl;
	if( lineNumber )
        cout << "Tokens: " << tokCount << endl;

	return 0;
}
