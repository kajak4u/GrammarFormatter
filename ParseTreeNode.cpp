#include "ParseTreeNode.h"

namespace Parser
{

	CParseTreeNode::CParseTreeNode(const CDefinedGrammarSymbol * identifier, const SubTree & subtree, CParsingState * state)
		: CParseTreeItem(state), identifier(identifier), subtree(subtree)
	{}

	CParseTreeNode::~CParseTreeNode()
	{
		for (auto& elem : subtree)
			delete elem.second;
	}

	const CDefinedGrammarSymbol * CParseTreeNode::GetIdentifier() const
	{
		return identifier;
	}

	const CParseTreeNode::SubTree & CParseTreeNode::GetSubTree() const
	{
		return subtree;
	}

}