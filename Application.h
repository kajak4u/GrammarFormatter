#pragma once

#include <string>
#include "Syntax.h"

class CApplication
{
	_STD string grammarFilename;
	_STD string codeFilename;
	void RegisterAllPrefixes();
	void UnregisterAllPrefixes();
	void ProcessCmdArgs(int argc, char* argv[]);
	GrammarSymbols::CSyntax ReadGrammar(_STD istream& file);
	void CheckCorrectness(GrammarSymbols::CSyntax& grammar);
public:
	CApplication(int argc, char* argv[])
	{
		ProcessCmdArgs(argc, argv);
	}

	void Run();
};