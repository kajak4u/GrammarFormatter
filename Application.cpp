#include "Application.h"

#include "Terminal.h"
#include "Group.h"
#include "MetaIdentifier.h"
#include "DefinedSymbolManager.h"
#include "Multiplier.h"
#include "Syntax.h"
#include "Special.h"
#include "main.h"
#include <regex>
#include <fstream>
#include <map>
#include "SyntaxRule.h"
#include "ParsingTable.h"
#include "Recognizer.h"

using namespace std;
using namespace GrammarSymbols;
using namespace Parser;

void CApplication::RegisterAllPrefixes()
{
	CTerminal::registerPrefixes();
	CGroup::registerPrefixes();
	CMetaIdentifier::registerPrefixes();
	CMultiplier::registerPrefixes();
	CSpecial::registerPrefixes();
}

void CApplication::UnregisterAllPrefixes()
{
	CRecognizer::FreeMemory();
}

void CApplication::ProcessCmdArgs(int argc, char* argv[])
{
	if (argc < 3)
		throw MYEXCEPTION("Too few arguments.", 1);
	grammarFilename = argv[1];
	codeFilename = argv[2];
	//if only 2 parameters, input file is either an output file
	outputFilename = argv[argc == 3 ? 2 : 3];
}

CSyntax CApplication::ReadGrammar(_STD istream & grammar)
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
		throw MYEXCEPTION(e.what(), 3);
	}
	return syntax;
}

void CApplication::CheckCorrectness(CSyntax & syntax)
{
	string message;
	if (!syntax.IsCorrect(ref(message)))
	{
		cerr << "Grammar contains errors!" << endl;
		throw MYEXCEPTION(message, 4);
	}
}

CApplication::CApplication(int argc, char * argv[])
{
	ProcessCmdArgs(argc, argv);
}
CApplication::~CApplication()
{
	UnregisterAllPrefixes();
}
void CApplication::Run()
{
	RegisterAllPrefixes();
	ifstream grammarFile(grammarFilename, ios::binary);
	if (!grammarFile.is_open())
		throw MYEXCEPTION("Could not open grammar file", 2);
	cout << "Reading grammar..." << endl;
	CSyntax grammar = ReadGrammar(grammarFile);
	grammarFile.close();
	cout << "Parsing grammar - step 1/3... ";
	cout.flush();
	grammar.Simplify();
	CheckCorrectness(grammar);
	cout << "\rParsing grammar - step 2/3... ";
	cout.flush();
	grammar.CreateSets();
#ifdef DEBUG_PRINTMEM
	cerr << "identifier memory:" << endl;
	CDefinedSymbolManager::PrintMemory(cerr);
	cout << grammar;
#endif
	cout << "\rParsing grammar - step 3/3..." << endl;
	CParsingTable table(grammar);
	cout << "Grammar parsed successfully" << endl;
#ifdef DEBUG_PRINTMEM
	cerr << "Parsing table: " << endl
		<< table << endl;
#endif

	ifstream codeFile(codeFilename, ios::binary);
	if (!codeFile.is_open())
		throw MYEXCEPTION("Could not open input file", 5);

	CParser parser(table[0]);
	cout << "Processing input file..." << endl;
	parser.Process(codeFile);
	codeFile.close();
#ifdef DEBUG_PRINTMEM
	cerr << "Parser stack: " << endl;
	parser.PrintStackTo(cerr);
#endif

	if (!parser.Accepted())
		throw MYEXCEPTION("The input file is not grammaticaly correct.", 6);
	ofstream output(outputFilename, ios::binary);
	if (!output.is_open())
		throw MYEXCEPTION("Could not open output file", 7);
	cout << "Saving input file..." << endl;
	parser.WriteFormattedTo(output);
	cout << "Done." << endl;
}