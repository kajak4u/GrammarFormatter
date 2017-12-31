#pragma once
#include "ParseTreeItem.h"
#include <vector>

namespace GrammarSymbols
{
	class CDefinedGrammarSymbol;
	class CPrimary;
}

namespace Parser
{
	using namespace ::GrammarSymbols;

	//class representing an internal node in parsing tree - associated with a symbol
	class CParseTreeNode : public CParseTreeItem
	{
	public:
		//node's subtree - an array of pairs [symbol -> derivation], 
		// where each symbol from [identifier]'s definition is associated to its derivation from input file
		using SubTree = std::vector<std::pair<CPrimary*, CParseTreeItem*>>;
	private:
		//symbol derivated from the subtree
		const CDefinedGrammarSymbol* identifier;
		//symbol's derivation
		SubTree subtree;
	public:
		//constructor
		CParseTreeNode(const CDefinedGrammarSymbol* identifier, const SubTree& subtree, CParsingState* state);
		//destructor
		virtual ~CParseTreeNode();
		//returns associated symbol
		const CDefinedGrammarSymbol* GetIdentifier() const;
		//returns symbol's derivation
		const SubTree& getSubTree() const;
	};
}
