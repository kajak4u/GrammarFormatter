#include "Application.h"
#include "Group.h"
#include "Multiplier.h"
#include "Recognizer.h"
#include "Parser.h"
#include "Special.h"
#include <fstream>

using namespace std;
using namespace GrammarSymbols;
using namespace Parser;

void CApplication::RegisterAllPrefixes()
{
	CGroup::RegisterPrefixes();
	CSpecial::RegisterPrefixes();
	CTerminal::RegisterPrefixes();
	CMultiplier::RegisterPrefixes();
	CMetaIdentifier::RegisterPrefixes();
}

void CApplication::UnregisterAllPrefixes()
{
	CRecognizer::FreeMemory();
}

void CApplication::ProcessCmdArgs(int argc, char* argv[])
{
	if (argc == 2 && (string(argv[1]) == "/?" || string(argv[1]) == "--help"))
	{
		
		throw MYEXCEPTION(string()+
			"Usage: GrammarFormatter grammar-file input-file [output-file]\n\n"+
			"Description: Uses EBNF grammar file to format input-file, save result to output-file\n\n"+
			"  grammar-file      file containing EBNF grammar file\n"+
			"  input-file        Input file to process\n"+
			"  output-file       Output file to store results.\n"+
			"                    If not specified, result is saved in input-file. ", 0);
	}
	if (argc < 3)
		throw MYEXCEPTION("Too few arguments. Use '/?' or '--help' for help.", -1);
	grammarFilename = argv[1];
	codeFilename = argv[2];
	//if there are only 2 parameters, input file is either an output file
	outputFilename = argv[argc == 3 ? 2 : 3];
}

CSyntax CApplication::ReadGrammar(_STD istream & grammar)
{
	cout << "Reading grammar..." << endl;
	CSyntax syntax;
	try
	{
		syntax.ReadFrom(grammar);
		cout << "Grammar loaded correctly" << endl;
	}
	catch (exception e)
	{
		PrintErrorPos(grammar, cerr);
		throw MYEXCEPTION(e.what(), 8);
	}
	return syntax;
}

void CApplication::CheckCorrectness(CSyntax & syntax)
{
	string message;
	if (!syntax.IsCorrect(ref(message)))
	{
		cerr << "Grammar contains errors!" << endl;
		throw MYEXCEPTION(message, -7);
	}
}

CParsingTable CApplication::ParseGrammar(GrammarSymbols::CSyntax & grammar)
{
	cout << "\rParsing grammar - step 1/3... ";
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
	return table;
}

void CApplication::ParseFile(Parser::CParser & parser, std::istream & file)
{
	cout << "Processing input file..." << endl;
	try
	{
		parser.Process(file);
	}
	catch (MyException& ex)
	{
		PrintErrorPos(file, cerr);
		throw;
	}
#ifdef DEBUG_PRINTMEM
	cerr << "Parser stack: " << endl;
	parser.PrintStackTo(cerr);
#endif
	if (!parser.Accepted())
		throw MYEXCEPTION("The input file is not grammaticaly correct.", -8);
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
	//read grammar
	RegisterAllPrefixes();
	ifstream grammarFile(grammarFilename, ios::binary);
	if (!grammarFile.is_open())
		throw MYEXCEPTION("Could not open grammar file", -9);
	CSyntax grammar = ReadGrammar(grammarFile);
	grammarFile.close();
	//parse grammar
	CParsingTable table = std::move(ParseGrammar(grammar));
	//parse input file
	ifstream codeFile(codeFilename, ios::binary);
	if (!codeFile.is_open())
		throw MYEXCEPTION("Could not open input file", -10);
	CParser parser(table[0]);
	ParseFile(parser, codeFile);
	codeFile.close();
	//write result
	ofstream output(outputFilename, ios::binary);
	if (!output.is_open())
		throw MYEXCEPTION("Could not open output file", -11);
	cout << "Saving input file..." << endl;
	parser.WriteFormattedTo(output);
	cout << "Done." << endl;
}