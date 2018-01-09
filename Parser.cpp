#include "Action.h"
#include "Goto.h"
#include "Parser.h"
#include "ParseTreeLeaf.h"
#include "ParseTreeNode.h"
#include "ParsingState.h"
#include "ParsingTable.h"
#include "Special.h"
#include "Syntax.h"
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
		//move proper number of items from stack to identifier's derivation subtree
		subtree.reserve(definition->size());
		auto separator = std::prev(stack.end(), definition->size());
		auto definitionIter = definition->begin();
		for (auto stackIter = separator; stackIter != stack.end(); ++definitionIter, ++stackIter)
			subtree.push_back({ *definitionIter, *stackIter });
		stack.erase(separator, stack.end());
		//check for special symbols
		if (const CSpecial* special = dynamic_cast<const CSpecial*>(identifier))
		{
			switch (special->GetFormat())
			{
				case FormatWhitespaceEnable:
					whitespacesAllowed = true;
					break;
				case FormatWhitespaceDisable:
					whitespacesAllowed = false;
					break;
			}
		}
		//change parser state and perform Goto
		currentState = stack.back()->GetState();
		stack.push_back(new CParseTreeNode(identifier, subtree, nullptr));
		currentState->GetGoto(identifier)->Perform(*this);
	}

	//prints derivation tree onto given stream
	void PrintTree(CParseTreeItem* tree, _STD ostream& os, int intend)
	{
		if (CParseTreeLeaf* leaf = dynamic_cast<CParseTreeLeaf*>(tree))
		{
			os << string(intend, ' ') << leaf->GetTerminal() << endl;
		}
		else if (CParseTreeNode* node = dynamic_cast<CParseTreeNode*>(tree))
		{
			os << string(intend, ' ') << node->GetIdentifier() << " => {" << endl;
			auto& subtree = node->GetSubTree();
			for (auto& item : subtree)
			{
				PrintTree(item.second, os, intend + 2);
			}
			os << string(intend, ' ') << "}" << endl;
		}
		else
			os << string(intend, ' ') << "[empty node]" << endl;
	}

	void CParser::PrintStackTo(_STD ostream& os) const
	{
		for (auto item : stack)
			PrintTree(item, os, 0);
	}

	void CParser::Process(istream & file)
	{
		SkipWhiteChars(file, false);
		bool lastRun = false;
		//when reached end of file, perform one last step with [uniqueTerminal] recognized
		while (!file.eof() || (lastRun = !lastRun))
		{
			if (lastRun)
				currentTerminal = CTerminal::Unique();
			else
				currentTerminal = CTerminal::Recognize(file);
			if (currentTerminal == nullptr)
				if (file.eof())
					continue;
				else
					throw MYEXCEPTION("Syntax error - unrecognized terminal.", -6);
#ifdef DEBUG_PARSING
			cerr << "Recognized terminal: " << *currentTerminal << endl;
#endif
			//'REDUCE' actions do not take terminal from input - repeat as long as it is still there
			while (currentTerminal != nullptr)
			{
				currentState->GetAction(currentTerminal)->Perform(*this);
#ifdef DEBUG_PARSING
				PrintStack();
#endif
			}
			if (whitespacesAllowed)
				SkipWhiteChars(file, false);
		}
	}


	CParser::CParser(CParsingState * startState)
		: currentState(startState)
	{
		stack.push_back(new CParseTreeItem(startState));
	}

	CParser::~CParser()
	{
		for (auto& elem : stack)
			delete elem;
	}

	void CParser::WriteFormattedTo(std::ostream & os) const
	{
		if (stack.size() != 2)
			throw MYEXCEPTION("Invalid stack size.",5);
		CParseTreeNode* treeTop = dynamic_cast<CParseTreeNode*>(stack.back());
		if (treeTop == nullptr)
			throw MYEXCEPTION("Invalid stack content.",6);
		int intend = 0;
		bool spaces = true;
		//stack of symbols with current positions - alternative for recursive printing
		vector<pair<CParseTreeNode::SubTree::const_iterator, CParseTreeNode::SubTree::const_iterator>> hierarchy = { 
			{ treeTop->GetSubTree().begin(), treeTop->GetSubTree().end() }
		};
		bool firstInLine = true;
		while (!hierarchy.empty())
		{
			//if reached end of subtree, pop it from stack's top
			if (hierarchy.back().first == hierarchy.back().second)
			{
				hierarchy.pop_back();
				continue;
			}
			//get current pos and increment it in background
			auto& elem = hierarchy.back().first++;
			//if special, apply format and print its derivation (if any)
			if (CSpecial* special = dynamic_cast<CSpecial*>(elem->first))
			{
				switch (special->GetFormat())
				{
				case FormatTab:
					os << "\t";
					firstInLine = true;
					break;
				case FormatWhitespaceEnable:
					spaces = true;
					break;
				case FormatWhitespaceDisable:
					spaces = false;
					break;
				case FormatNewLine:
					os << "\n" << string(intend, '\t');
					firstInLine = true;
					break;
				case FormatNoSpace:
					firstInLine = true;
					break;
				case FormatIndent:
					++intend;
					break;
				case FormatDedent:
					if (intend)
						--intend;
					break;
				default:
					break;
				}
				//if special symbol has a derivation, print it
				if (CParseTreeNode* node = dynamic_cast<CParseTreeNode*>(elem->second))
					hierarchy.push_back({ node->GetSubTree().begin(), node->GetSubTree().end() });
			}
			//if terminal, print it with current format
			else if (CTerminal* terminal = dynamic_cast<CTerminal*>(elem->first))
			{
				if (firstInLine)
					firstInLine = false;
				else
					os << (spaces ? " " : "");
				os << terminal->GetValue();
			}
			//if identifier, print its derivation
			else if (CMetaIdentifier* identifier = dynamic_cast<CMetaIdentifier*>(elem->first))
			{
				if (CParseTreeNode* node = dynamic_cast<CParseTreeNode*>(elem->second))
					hierarchy.push_back({ node->GetSubTree().begin(), node->GetSubTree().end() });
			}
			//shouldn't ever happen
			else
				throw MYEXCEPTION("Unexpected node in hierarchy", 7);
		}
	}

}