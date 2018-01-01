#pragma once

#include <string>
#include "Syntax.h"
#include "ParsingTable.h"

namespace Parser
{
	class CParser;
}

//main program class
class CApplication
{
	//path to file containing grammar
	_STD string grammarFilename;
	//path to file to process
	_STD string codeFilename;
	//result file path
	_STD string outputFilename;
	//calls all classes' RegisterPrefixes() methods
	void RegisterAllPrefixes();
	//frees the CRecognizer prediction memory
	void UnregisterAllPrefixes();
	//extracts data from commandline arguments
	void ProcessCmdArgs(int argc, char* argv[]);
	//returns grammar read from given file
	GrammarSymbols::CSyntax ReadGrammar(_STD istream& file);
	//checks grammar's correctness
	void CheckCorrectness(GrammarSymbols::CSyntax& grammar);
	//creates parsing table from grammar
	Parser::CParsingTable ParseGrammar(GrammarSymbols::CSyntax& grammar);
	//parses input file using given parser
	void ParseFile(Parser::CParser& parser, _STD istream& file);
public:
	//constructor with commandline arguments
	CApplication(int argc, char* argv[]);
	//destructor
	~CApplication();
	//project's main method
	void Run();
};