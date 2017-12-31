#include "main.h"
#include "Application.h"
#include <fstream>
#include <string>
#include <map>

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

void ExtractLinePos(istream& is, int& line, int& pos, string& lineContent)
{
	_STD streamoff filepos = is.tellg();
	is.seekg(0, is.beg);
	line = 0;
	string str;
	_STD streamoff oldfilepos = 0;
	do
	{
		oldfilepos = is.tellg();
		++line;
		getline(is, str, '\n');
	} while (is.tellg() < filepos);

	pos = int(filepos - oldfilepos);
	is.seekg(filepos, is.beg);
	lineContent = str;
}

void skipComment(_STD istream& is)
{
	string s = { (char)is.get(), (char)is.get() };
	if (s != "(*")
	{
		is.putback(s[1]);
		is.putback(s[0]);
		return;
	}
	char c;
	while (!is.eof())
	{
		switch (c = is.get())
		{
		case '*':
			if ((c = is.get()) == ')')
				return;
			else
				is.putback(c);
			break;
		case '(':
			if ((c = is.get()) == '*')
			{
				is.putback(c);
				is.putback('(');
				skipComment(is);
			}
			else
				is.putback(c);
			break;
		}
	}
}

void skipWhiteChars(_STD istream& is, bool skipEBNFComment)
{
	char c, c2;
	while (!is.eof())
		switch (c = is.get())
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			break;
		case '(':
			if (skipEBNFComment)
			{
				c2 = is.get();
				if (c2 == '*')
				{
					is.putback(c2);
					is.putback(c);
					skipComment(is);
					break;
				}
				is.putback(c2);
			}
		default:
			if(c!=-1)
				is.putback(c);
			return;
		}
}

Symbol GetSymbol(_STD istream & is, int & group, bool alterStream)
{
}

Symbol GetSymbol(_STD istream & is, bool alterStream)
{
	static const map<string, Symbol> symbols =
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
	skipWhiteChars(is);
	char c = is.get();
	char c2 = is.get();
	auto result = symbols.find(string{ c, c2 });
	if (result == symbols.cend())
		result = symbols.find(string{ c });
	if (!alterStream)
	{
		is.putback(c2);
		is.putback(c);
	}
	else if (result != symbols.end() && result->first.length() == 1)
		is.putback(c2);
	if (result == symbols.cend())
	{
		return SymbolUnknown;
	}
	return result->second;
}


int main(int argc, char* argv[])
{
	try
	{
		CApplication app(argc, argv);
		app.Run();
#ifdef DEBUG_MEMLEAKS
		GrammarSymbols::CGrammarObject::PrintCounter();
#endif
		cout << endl;
		system("pause");
		return 0;
	}
	catch (MyException& e)
	{
		cerr << e.message << endl;
		system("pause");
		return e.retCode;
	}
}