#include "main.h"
#include "Terminal.h"
#include "Group.h"
#include "MetaIdentifier.h"
#include "Multiplier.h"
#include "Syntax.h"
#include <fstream>
#include <string>

using namespace std;

void ExtractLinePos(istream& is, int& line, int& pos)
{
	std::streamoff filepos = is.tellg();
	is.seekg(0, is.beg);
	line = 0;
	string str;
	vector<pair<string, std::streamoff>> lines;
	std::streamoff oldfilepos = 0;
	do
	{
		oldfilepos = is.tellg();
		++line;
		getline(is, str, '\n');
		lines.push_back({ str, is.tellg() });
	} while (is.tellg() < filepos);

	pos = int(filepos - oldfilepos);
	is.seekg(filepos, is.beg);
}

int main(int argc, char* argv[])
{
	CTerminal::registerPrefixes();
	CGroup::registerPrefixes();
	CMetaIdentifier::registerPrefixes();
	CMultiplier::registerPrefixes();

	if (argc < 3)
		return -1;
	ifstream grammar(argv[1]);
	if (!grammar.is_open())
		return -2;
	CSyntax syntax;
	try
	{
		syntax.ReadFrom(grammar);
		cout << "Grammar  loaded correctly" << endl;
	}
	catch (exception e)
	{
		int line, pos;
		std::streamoff p1 = grammar.tellg();
		char c1 = grammar.get();
		grammar.putback(c1);
		ExtractLinePos(grammar, line, pos);
		std::streamoff p2 = grammar.tellg();
		char c2 = grammar.get();
		cerr << "Error in line " << line << ", char " << pos << endl;
		cerr << e.what() << endl;
	}
	system("pause");
	return 0;
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
	while (1)
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
	while (1)
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
			}
			break;
		default:
			is.putback(c);
			return;
		}
}

#include <map>
using namespace std;

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
