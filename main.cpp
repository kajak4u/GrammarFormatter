#include "main.h"
#include "Application.h"
#include <fstream>
#include <string>
#include <map>

using namespace std;

void ExtractLinePos(istream& is, int& line, int& pos, string& lineContent)
{
	std::streamoff filepos = is.tellg();
	is.seekg(0, is.beg);
	line = 0;
	string str;
	std::streamoff oldfilepos = 0;
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

void skipComment(std::istream& is)
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

void skipWhiteChars(std::istream& is)
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
			c2 = is.get();
			if (c2 == '*')
			{
				is.putback(c2);
				is.putback(c);
				skipComment(is);
				break;
			}
			is.putback(c2);
		default:
			if(c!=-1)
				is.putback(c);
			return;
		}
}

Symbol GetSymbol(std::istream & is, int & group, bool alterStream)
{
	static const map<string, pair<Symbol,int>> symbols = 
	{
		{ ",",{ SymbolConcatenate,	0 } },
		{ "=",{ SymbolDefining,		0 } },
		{ "|",{ SymbolSeparator,	0 } },
		{ "\\",{ SymbolSeparator,	1 } },
		{ "!",{ SymbolSeparator,	2 } },
		{ "*)",{ SymbolCommentEnd,	0 } },
		{ "(*",{ SymbolCommentStart,0 } },
		{ ")",{ SymbolGroupEnd,		0 } },
		{ "(",{ SymbolGroupStart,	0 } },
		{ "]",{ SymbolOptionEnd,	0 } },
		{ "/)",{ SymbolOptionEnd,	1 } },
		{ "[",{ SymbolOptionStart,	0 } },
		{ "(/",{ SymbolOptionStart,	1 } },
		{ "}",{ SymbolRepeatEnd,	0 } },
		{ ":)",{ SymbolRepeatEnd,	1 } },
		{ "{",{ SymbolRepeatStart,	0 } },
		{ "(:",{ SymbolRepeatStart,	1 } },
		{ "-",{ SymbolExcept,		0 } },
		{ "'",{ SymbolQuote,		0 } },
		{ "\"",{ SymbolQuote,		1 } },
		{ "*",{ SymbolRepetition,	0 } },
		{ "?",{ SymbolSpecial,		0 } },
		{ ";",{ SymbolTerminator,	0 } },
		{ ".",{ SymbolTerminator,	1 } }
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
		group = -1;
		return SymbolUnknown;
	}
	group = result->second.second;
	return result->second.first;
}

Symbol GetSymbol(std::istream & is, bool alterStream)
{
	int gr;
	return GetSymbol(is, gr, alterStream);
}


int main(int argc, char* argv[])
{
	try
	{
		CApplication app(argc, argv);
		app.Run();
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