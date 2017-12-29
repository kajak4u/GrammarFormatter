#include "Parser.h"

#include "Syntax.h"
#include "main.h"
#include "ParsingTable.h"
#include "Special.h"
using namespace std;

void CParser::ChangeStateTo(CParsingState * state)
{
	stack.back()->SetState(state);
	currentState = state;
}

void CParser::AddStateToStack(CParsingState * newState)
{
	stack.push_back(new CLeaf(currentTerminal, newState));
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
	CNode::SubTree subtree;
	subtree.reserve(definition->size());
	auto separator = std::prev(stack.end(), definition->size());
	auto definitionIter = definition->begin();
	for (auto stackIter = separator; stackIter != stack.end(); ++definitionIter)
		subtree.push_back({ *definitionIter, *stackIter++ });
	stack.erase(separator, stack.end());
	currentState = stack.back()->GetState();
	auto nextStateIter = currentState->gotos.find(identifier);
	if (nextStateIter == currentState->gotos.end())
		throw MyException(string() + "Syntax error - unexpected symbol " + identifier->GetName() + ".", 0);
	stack.push_back(new CNode(identifier, subtree, nullptr));
	nextStateIter->second->Perform(*this);
}

#include "ParsingTable.h"

void PrintDrzewo(CDrzewo* item, int intend)
{
	if (CLeaf* leaf = dynamic_cast<CLeaf*>(item))
	{
		cerr << string(intend, ' ') << *leaf->GetTerminal() << endl;
	}
	else if (CNode* node = dynamic_cast<CNode*>(item))
	{
		cerr << string(intend, ' ') << *node->GetIdentifier() << " => {" << endl;
		auto& subtree = node->getSubTree();
		for (auto& subtreeitem : subtree)
		{
			PrintDrzewo(subtreeitem.second, intend + 2);
		}
		cerr << string(intend, ' ') << "}" << endl;
	}
	else
		cerr << string(intend, ' ') << "[empty node]" << endl;
}

void PrintStack(const std::vector<CDrzewo*>& stack)
{
	for (auto item : stack)
	{
		PrintDrzewo(item, 0);
	}
}

void CParser::Process(istream & file)
{
	skipWhiteChars(file);
	bool lastRun = false;
	while (!file.eof() || (lastRun=!lastRun))
	{
		if (lastRun)
			currentTerminal = CTerminal::Unique();
		else
			currentTerminal = CTerminal::Recognize(file);
		if (currentTerminal == nullptr)
			throw MyException("Syntax error - unrecognized terminal.\n" __FILE__, __LINE__);
#ifdef DEBUG_PARSING
		cerr << "Recognized terminal: " << *currentTerminal << endl;
#endif
		skipWhiteChars(file);
		while (currentTerminal != nullptr)
		{
			auto iter = currentState->actions.find(currentTerminal);
			if (iter == currentState->actions.end())
				throw MyException(string() + "Syntax error - unexpected terminal " + currentTerminal->GetValue() + ".", 0);
			iter->second->Perform(*this);
#ifdef DEBUG_PARSING
			PrintStack(stack);
#endif
		}
	}
}


CParser::CParser(CParsingState * startState)
	: currentState(startState)
{
	stack.push_back(new CDrzewo(startState));
}

CParser::~CParser()
{
	for (auto& elem : stack)
		delete elem;
}

_STD ostream & operator<<(_STD ostream & os, const CSituation & situation)
{
	os << *situation.result << " =";
	for (auto iter = situation.def->begin(); iter != situation.def->end(); ++iter)
	{
		os << (iter == situation.pos ? " * " : " ");
		if (*iter == nullptr)
			os << "[empty]";
		else
			(*iter)->WriteTo(os);
	}
	if (situation.pos == situation.def->end())
		os << " *";
	os << ", " << *situation.allowed;
	return os;
}

const CDefinedGrammarSymbol * CNode::GetIdentifier() const
{
	return identifier;
}

const CNode::SubTree & CNode::getSubTree() const
{
	return subtree;
}

const CTerminal * CLeaf::GetTerminal() const
{
	return terminal;
}

void CParser::WriteFormattedTo(std::ostream & os) const
{
	if (stack.size() != 2)
		throw MyException("Invalid stack size.\n" __FILE__, __LINE__);
	CNode* treeTop = dynamic_cast<CNode*>(stack.back());
	if (treeTop == nullptr)
		throw MyException("Invalid stack content.\n" __FILE__, __LINE__);
	int intend = 0;
	bool spaces = true;
	vector<pair<CNode::SubTree::const_iterator, CNode::SubTree::const_iterator>> hierarchy = { { treeTop->getSubTree().begin(), treeTop->getSubTree().end() } };
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
				if(intend)
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
			if(CNode* node = dynamic_cast<CNode*>(elem->second))
				hierarchy.push_back({ node->getSubTree().begin(), node->getSubTree().end() });
		}
		else
			throw MyException("Unexpected node in hierarchy\n" __FILE__, __LINE__);
	}
}
