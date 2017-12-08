#include "Application.h"

#include "Terminal.h"
#include "Group.h"
#include "MetaIdentifier.h"
#include "Multiplier.h"
#include "Syntax.h"
#include "Special.h"
#include "main.h"
#include <regex>
#include <fstream>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "SyntaxRule.h"

using namespace std;

void CApplication::RegisterAllPrefixes()
{
	CTerminal::registerPrefixes();
	CGroup::registerPrefixes();
	CMetaIdentifier::registerPrefixes();
	CMultiplier::registerPrefixes();
	CSpecial::registerPrefixes();
}

void CApplication::ProcessCmdArgs(int argc, char* argv[])
{
	if (argc < 3)
		throw MyException("Too few arguments.", -1);
	grammarFilename = argv[1];
	codeFilename = argv[2];
}

CSyntax CApplication::ReadGrammar(std::istream & grammar)
{
	CSyntax syntax;
	try
	{
		syntax.ReadFrom(grammar);
		cout << "Grammar loaded correctly" << endl;
	}
	catch (exception e)
	{
		int line, pos;
		string lineContent;
		ExtractLinePos(grammar, line, pos, lineContent);
		lineContent = regex_replace(lineContent, regex("\t"), " ");
		cerr << "Error in line " << line << ", char " << pos << endl
			<< lineContent << endl;
		if (pos != 0)
			cerr << string(pos - 1, '=') << "^" << endl;
		throw MyException(e.what(), -3);
	}
	return syntax;
}

void CApplication::CheckCorrectness(CSyntax & syntax)
{
	string message;
	if (!syntax.IsCorrect(ref(message)))
	{
		cerr << "Grammar contains errors!" << endl;
		throw MyException(message, -4);
	}

	cout << "Grammar is correct" << endl;
	if (message != "")
		cout << "warnings: " << endl
		<< message << endl;
}

#include "MetaIdentifierManager.h"
void CApplication::Run()
{
	RegisterAllPrefixes();
	ifstream grammarFile(grammarFilename, ios::binary);
	if (!grammarFile.is_open())
		throw MyException("Could not open grammar file", -2);
	CSyntax grammar = ReadGrammar(grammarFile);
	grammarFile.close();
	grammar.Simplify();

	CheckCorrectness(grammar);
	grammar.CreateSets();

	CMetaIdentifierManager::PrintMemory();

	cout << grammar;

	ifstream codeFile(codeFilename, ios::binary);
	if (!codeFile.is_open())
		throw MyException("Could not open input file", -5);
}