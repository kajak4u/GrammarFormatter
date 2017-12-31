#pragma once

#include <string>
#include "Syntax.h"

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
public:
	//constructor with commandline arguments
	CApplication(int argc, char* argv[]);
	//project's main method
	void Run();
};