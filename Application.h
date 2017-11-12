#pragma once

#include <string>
#include "Syntax.h"
#include "ParsingTable.h"

class CApplication
{
	std::string grammarFilename;
	std::string codeFilename;
	void RegisterAllPrefixes();
	void ProcessCmdArgs(int argc, char* argv[]);
	CSyntax ReadGrammar(std::istream& file);
	void CheckCorrectness(const CSyntax& grammar);
	CParsingTable CreateParsingTable(const CSyntax& grammar);
public:
	CApplication(int argc, char* argv[])
	{
		ProcessCmdArgs(argc, argv);
	}

	void Run();
};