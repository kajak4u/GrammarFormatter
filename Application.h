#pragma once

#include <string>
#include "Syntax.h"

class CApplication
{
	std::string grammarFilename;
	std::string codeFilename;
	void RegisterAllPrefixes();
	void ProcessCmdArgs(int argc, char* argv[]);
	CSyntax ReadGrammar(std::istream& file);
	void CheckCorrectness(CSyntax& grammar);
public:
	CApplication(int argc, char* argv[])
	{
		ProcessCmdArgs(argc, argv);
	}

	void Run();
};