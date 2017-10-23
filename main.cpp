#include "main.h"
#include "Terminal.h"
#include "Group.h"
#include "MetaIdentifier.h"
#include "Multiplier.h"
#include "Syntax.h"
#include <fstream>

using namespace std;

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
		cerr << e.what() << endl;
	}
	system("pause");
	return 0;
}

void skipWhiteChars(std::istream& is)
{
	char c;
	while (1)
		switch (c = is.get())
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
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
