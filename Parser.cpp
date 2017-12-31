#include "Parser.h"

#include "Syntax.h"
#include "main.h"
#include "ParsingTable.h"
#include "ParseTreeLeaf.h"
#include "Special.h"
#include "ParsingState.h"
#include "Action.h"
#include "Goto.h"
using namespace std;

namespace Parser
{

	void CParser::ChangeStateTo(CParsingState * state)
	{
		stack.back()->SetState(state);
		currentState = state;
	}

	void CParser::AddStateToStack(CParsingState * newState)
	{
		stack.push_back(new CParseTreeLeaf(currentTerminal, newState));
		currentState = newState;
		currentTerminal = nullptr;
	}

	void CParser::Accept()
	{
		accepted = true;
		currentTerminal = nullptr;
	}

	bool CParser::Accepted() const
	{
		return accepted;
	}

	void CParser::Reduce(const CDefinedGrammarSymbol *identifier, const CShortDefinition *definition)
	{
		CParseTreeNode::SubTree subtree;
		subtree.reserve(definition->size());
		auto separator = std::prev(stack.end(), definition->size());
		auto definitionIter = definition->begin();
		for (auto stackIter = separator; stackIter != stack.end(); ++definitionIter, ++stackIter)
			subtree.push_back({ *definitionIter, *stackIter });
		stack.erase(separator, stack.end());
		currentState = stack.back()->GetState();
		stack.push_back(new CParseTreeNode(identifier, subtree, nullptr));
		currentState->GetGoto(identifier)->Perform(*this);
	}

	void PrintTree(CParseTree* tree, int intend)
	{
		if (CParseTreeLeaf* leaf = dynamic_cast<CParseTreeLeaf*>(tree))
		{
			cerr << string(intend, ' ') << *leaf->GetTerminal() << endl;
		}
		else if (CParseTreeNode* node = dynamic_cast<CParseTreeNode*>(tree))
		{
			cerr << string(intend, ' ') << *node->GetIdentifier() << " => {" << endl;
			auto& subtree = node->getSubTree();
			for (auto& item : subtree)
			{
				PrintTree(item.second, intend + 2);
			}
			cerr << string(intend, ' ') << "}" << endl;
		}
		else
			cerr << string(intend, ' ') << "[empty node]" << endl;
	}

	void PrintStack(const std::vector<CParseTree*>& stack)
	{
		for (auto item : stack)
		{
			PrintTree(item, 0);
		}
	}

	void CParser::Process(istream & file)
	{
		skipWhiteChars(file, false);
		bool lastRun = false;
		while (!file.eof() || (lastRun = !lastRun))
		{
			if (lastRun)
				currentTerminal = CTerminal::Unique();
			else
				currentTerminal = CTerminal::Recognize(file);
			if (currentTerminal == nullptr)
				throw MYEXCEPTION("Syntax error - unrecognized terminal.\n" __FILE__, __LINE__);
#ifdef DEBUG_PARSING
			cerr << "Recognized terminal: " << *currentTerminal << endl;
#endif
			skipWhiteChars(file, false);
			while (currentTerminal != nullptr)
			{
				currentState->GetAction(currentTerminal)->Perform(*this);
#ifdef DEBUG_PARSING
				PrintStack(stack);
#endif
			}
		}
	}


	CParser::CParser(CParsingState * startState)
		: currentState(startState)
	{
		stack.push_back(new CParseTree(startState));
	}

	CParser::~CParser()
	{
		for (auto& elem : stack)
			delete elem;
	}

	void CParser::WriteFormattedTo(std::ostream & os) const
	{
		if (stack.size() != 2)
			throw MYEXCEPTION("Invalid stack size.",1);
		CParseTreeNode* treeTop = dynamic_cast<CParseTreeNode*>(stack.back());
		if (treeTop == nullptr)
			throw MYEXCEPTION("Invalid stack content.",1);
		int intend = 0;
		bool spaces = true;
		vector<pair<CParseTreeNode::SubTree::const_iterator, CParseTreeNode::SubTree::const_iterator>> hierarchy = { { treeTop->getSubTree().begin(), treeTop->getSubTree().end() } };
		bool firstInLine = true;
		while (!hierarchy.empty())
		{
			if (hierarchy.back().first == hierarchy.back().second)
			{
				hierarchy.pop_back();
				continue;
			}
			auto& elem = hierarchy.back().first++;
			if (CSpecial* special = dynamic_cast<CSpecial*>(elem->first))
			{
				switch (special->getFormat())
				{
				case FormatTab:
					os << "\t";
					break;
				case FormatSpace:
					spaces = true;
					break;
				case FormatNoSpace:
					spaces = false;
					break;
				case FormatNewLine:
					os << "\n" << string(intend * 2, ' ');
					firstInLine = true;
					break;
				case FormatIntend:
					++intend;
					break;
				case FormatDedend:
					if (intend)
						--intend;
					break;
				default:
					break;
				}
			}
			else if (CTerminal* terminal = dynamic_cast<CTerminal*>(elem->first))
			{
				if (firstInLine)
					firstInLine = false;
				else
					os << (spaces ? " " : "");
				os << terminal->GetValue();
			}
			else if (CMetaIdentifier* identifier = dynamic_cast<CMetaIdentifier*>(elem->first))
			{
				if (CParseTreeNode* node = dynamic_cast<CParseTreeNode*>(elem->second))
					hierarchy.push_back({ node->getSubTree().begin(), node->getSubTree().end() });
			}
			else
				throw MYEXCEPTION("Unexpected node in hierarchy", 1);
		}
	}

}