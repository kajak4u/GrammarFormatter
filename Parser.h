#pragma once

#include "ShortDefinition.h"
#include "Terminal.h"
#include <iostream>
#include "ParseTree.h"
#include "ParseTreeNode.h"

namespace GrammarSymbols
{
	class CDefinedGrammarSymbol;
}

namespace Parser
{
	class CParsingState;

	//class performing the parsing process, starting from given state
	class CParser
	{
		//parser's current state
		CParsingState* currentState;
		//whether input was accepted
		bool accepted = false;
		//parser's stack
		std::vector<CParseTree*> stack;
		//recently read terminal, waiting for processing
		CTerminal* currentTerminal = nullptr;
	public:
		//changes current state to given [state]
		void ChangeStateTo(CParsingState* state);
		//pushes current state to stack and sets [newState] as new current state
		void AddStateToStack(CParsingState* newState);
		//accepts input
		void Accept();
		//returns whether input was accepted
		bool Accepted() const;
		//reduces stack by rule [result] --> [definition]
		void Reduce(const CDefinedGrammarSymbol* result, const CShortDefinition* definition);
		//processes given input
		void Process(_STD istream& is);

		//constructor, sets initial parser's state
		CParser(CParsingState* startState);
		//apply format to input's derivation tree and print it to given output
		void WriteFormattedTo(_STD ostream& os) const;
		//destructor
		~CParser();
	};
}