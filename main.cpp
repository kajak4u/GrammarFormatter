#include "Application.h"
#include "main.h"
#include "Reverse.h"
#include <map>
#include <regex>

/*
 * available debug macros:
 * DEBUG_MEMLEAKS - prints CGrammarSymbol usage
 * DEBUG_PARSINGTABLE - prints steps of creating parsing table
 * DEBUG_SIMPLIFY - prints steps of rules simplifying
 * DEBUG_PRINTMEM - prints content of collections
 * DEBUG_PARSING - prints steps of parsing process
 *
**/

using namespace std;

 void PrintErrorPos(std::istream & file, std::ostream& output)
 {
	 int line, pos;
	 string lineContent;
	 ExtractLinePos(file, line, pos, lineContent);
	 //replace tabs to spaces, so that string length is equal to number of characters
	 lineContent = regex_replace(lineContent, regex("\t"), " ");
	 output << "Error in line " << line << ", char " << pos << endl
		 << lineContent << endl;
	 // if pos is 0, there is an error and program is unable to point the character
	 if (pos != 0)
		 output << string(pos - 1, '=') << "^" << endl;
 }

 void ExtractLinePos(istream& is, int& line, int& pos, string& lineContent)
{
	 line = 0;
	 string str;
	 //get current stream pos
	_STD streamoff filepos = is.tellg();
	//go to begin and read lines until exceeded start position
	is.seekg(0, is.beg);
	_STD streamoff oldfilepos = 0;
	do
	{
		oldfilepos = is.tellg();
		++line;
		getline(is, str, '\n');
	} while (!is.eof() && is.tellg() < filepos);
	//offset in line is the difference between start position and the line's begin
	pos = int(filepos - oldfilepos);
	lineContent = str;
	//return to start position
	is.seekg(filepos, is.beg);
}

void SkipComment(_STD istream& is)
{
	//EBNF comments are cumulative - each start symbol '(*' must have its closing equivalent '*)'
	//  e.g. (* ... (* ... *) ... *)
	string s = { (char)is.get(), (char)is.get() };
	//if not comment, return chars to stream and return
	if (s != "(*")
	{
		is.putback(s[1]);
		is.putback(s[0]);
		return;
	}
	char c;
	while (!is.eof())
	{
		//waiting for *)
		switch (c = is.get())
		{
		case '*':
			if ((c = is.get()) == ')')
				return;
			else
				is.putback(c);
			break;
		case '(':
			//check whether encountered nested comment
			if ((c = is.get()) == '*')
			{
				is.putback(c);
				is.putback('(');
				SkipComment(is);
			}
			else
				is.putback(c);
			break;
		}
	}
}

void SkipWhiteChars(_STD istream& is, bool skipEBNFComment)
{
	char c, c2;
	while (!is.eof())
		switch (c = is.get())
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			//skip whitespaces
			break;
		case '(':
			if (skipEBNFComment)
			{
				//check if it is comment, then skip it
				c2 = is.get();
				if (c2 == '*')
				{
					is.putback(c2);
					is.putback(c);
					SkipComment(is);
					break;
				}
				is.putback(c2);
			}
		default:
			//not a whitespace - put it back and return
			if(c!=-1)
				is.putback(c);
			return;
		}
}

pair<string, Symbol> RecognizeSymbol(_STD istream& is)
{
	//static dictionary of all symbols and their matchings
	const map<string, Symbol> symbols =
	{
		{ ",",	SymbolConcatenate },
		{ "=",	SymbolDefining },
		{ "|",	SymbolSeparator },
		{ "\\",	SymbolSeparator },
		{ "!",	SymbolSeparator },
		{ "*)",	SymbolCommentEnd },
		{ "(*",	SymbolCommentStart, },
		{ ")",	SymbolGroupEnd },
		{ "(",	SymbolGroupStart },
		{ "]",	SymbolOptionEnd },
		{ "/)",	SymbolOptionEnd },
		{ "[",	SymbolOptionStart },
		{ "(/",	SymbolOptionStart },
		{ "}",	SymbolRepeatEnd },
		{ ":)",	SymbolRepeatEnd },
		{ "{",	SymbolRepeatStart },
		{ "(:",	SymbolRepeatStart },
		{ "-",	SymbolExcept },
		{ "'",	SymbolQuote },
		{ "\"",	SymbolQuote },
		{ "*",	SymbolRepetition },
		{ "?",	SymbolSpecial },
		{ ";",	SymbolTerminator },
		{ ".",	SymbolTerminator }
	};
	SkipWhiteChars(is);
	//get 2 chars, for symbols are 1- or 2-characters
	char c = is.get();
	char c2 = is.get();
	auto result = symbols.find({ c, c2 });
	if (result != symbols.end())
		return *result;
	//if 2-character symbol is not matched, try with 1-character
	is.putback(c2);
	result = symbols.find({ c });
	if (result != symbols.end())
		return *result;

	//if nothing matched, return [Unknown]
	return{ "", SymbolUnknown };
}

Symbol GetSymbol(_STD istream & is, bool alterStream)
{
	pair<string,Symbol> result = RecognizeSymbol(is);
	if (!alterStream)
		for (char c : Reversed(result.first))
			is.putback(c);
	return result.second;
}

bool TakeSymbolIf(_STD istream& is, Symbol expectedSymbol)
{
	pair<string, Symbol> result = RecognizeSymbol(is);
	if (result.second == expectedSymbol)
		return true;
	//invalid symbol, so put characters back
	for (char c : Reversed(result.first))
		is.putback(c);
	return false;
}


int main(int argc, char* argv[])
{
	CApplication* app = nullptr;
	int retcode = 0;
	//run application
	try
	{
		app = new CApplication(argc, argv);
		app->Run();
		delete app;
		app = nullptr;
#ifdef DEBUG_MEMLEAKS
		GrammarSymbols::CGrammarObject::PrintCounter();
#endif
		cout << endl;
	}
	catch (MyException& e)
	{
		//in case of error, print it and free memory if necessary
		cerr << e.message << endl;
		if (app)
			delete app;
		retcode = e.retCode;
	}
#ifdef _DEBUG
	//in debug mode, pause console before exiting
	system("pause");
#endif
	return retcode;
}