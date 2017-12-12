#include "Parser.h"

#include "Syntax.h"
#include "main.h"
#include "ParsingTable.h"
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

void CParser::Reduce(const CMetaIdentifier *identifier, const CShortDefinition *definition)
{
	CNode::SubTree subtree;
	subtree.reserve(definition->size());
	auto separator = std::prev(stack.end(), definition->size());
	auto definitionIter = definition->begin();
	for (auto stackIter = separator; stackIter != stack.end(); ++stackIter, ++definitionIter)
		subtree.push_back({ *definitionIter, *stackIter });
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
			/*cerr << string(intend + 2, ' ');
			if (subtreeitem.first != nullptr)
				subtreeitem.first->WriteTo(cerr);
			else
				cerr << " [empty]";
			cerr << " = " << endl;*/
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
	//Recognize between 
	//	SELECT (SEL) as field
	//	SELECT (SELECT xkey FROM sslp LIMIT 1)
	skipWhiteChars(file);
	bool lastRun = false;
	while (!file.eof() || (lastRun=!lastRun))
	{
		if (lastRun)
			currentTerminal = CTerminal::Unique();
		else
			currentTerminal = CTerminal::Recognize(file);
		cerr << "Recognized terminal: " << *currentTerminal << endl;
		skipWhiteChars(file);
		while (currentTerminal != nullptr)
		{
			auto iter = currentState->actions.find(currentTerminal);
			if (iter == currentState->actions.end())
				throw MyException(string() + "Syntax error - unexpected terminal " + currentTerminal->GetValue() + ".", 0);
			iter->second->Perform(*this);

			PrintStack(stack);
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

const CMetaIdentifier * CNode::GetIdentifier() const
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
