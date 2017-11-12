#include "ParsingTable.h"
#include "SyntaxRule.h"
#include "Term.h"
#include "Definition.h"
#include "main.h"
#include "Group.h"
#include "Special.h"

using namespace std;

void CParsingTable::ParseIdentifier(const CMetaIdentifier & identifier)
{
	IdentifiersInMemory[identifier] = nullptr;
	auto& identifierRules = rulesMap.find(identifier)->second;
	int ruleOrder = size();
	this->push_back(new CParserAction());
	cout << "Identifier " << identifier << " rule -> " << ruleOrder << endl;
	for (auto& rule : identifierRules)
		for (const CDefinition* definition : *rule.second)
		{
			RulesList endSymbols = Parse(RulesList{ ruleOrder }, definition);
			for (auto elem : endSymbols)
			{
				(*this)[elem]->reductionRule = rule.first;
			}
		}
	IdentifiersInMemory[identifier] = (*this)[ruleOrder];
}

CParsingTable::CParsingTable()
{
}

CParsingTable::CParsingTable(const CParsingTable & other)
{
}

CParsingTable::RulesList CParsingTable::Parse(const RulesList& currentRules, const CDefinition * definition)
{
	RulesList result = currentRules;
	for (const CTerm* term : *definition)
		result = Parse(result, term);
	return result;
}

CParsingTable::RulesList CParsingTable::Parse(const RulesList& currentRules, const CTerm * term)
{
	RulesList newRules;
	const CFactor& factor = term->GetFactor();
	const CPrimary* primary = factor.GetPrimary();
	if (const CMetaIdentifier* identifier = dynamic_cast<const CMetaIdentifier*>(primary))
	{
		for (auto elem : currentRules)
		{
			CParserAction* currentAction = (*this)[elem];

			if (IdentifiersInMemory.find(*identifier) == IdentifiersInMemory.end())
				ParseIdentifier(*identifier);
			CParserAction* identifierRecord = IdentifiersInMemory[*identifier];
			if (identifierRecord != nullptr)
			{
				if (identifierRecord->actions != nullptr)
					for (auto& elem : *identifierRecord->actions)
						currentAction->AddShiftAction(elem.first, elem.second);
				if (identifierRecord->gotos != nullptr)
					for (auto& elem : *identifierRecord->gotos)
						currentAction->AddGoto(elem.first, elem.second);
			}

			int gotoAction = currentAction->GetGoto(identifier);
			if (gotoAction != -1)
				newRules.insert(gotoAction);
			else
			{
				currentAction->AddGoto(identifier, size());
				newRules.insert(size());
				this->push_back(new CParserAction());
			}
		}
	}
	else if (const CTerminal* terminal = dynamic_cast<const CTerminal*>(primary))
	{
		for (auto elem : currentRules)
		{
			CParserAction* currentAction = (*this)[elem];
			int shiftAction = currentAction->GetShiftAction(terminal);
			if (shiftAction != -1)
				newRules.insert(shiftAction);
			else
			{
				currentAction->AddShiftAction(terminal, size());
				newRules.insert(size());
				this->push_back(new CParserAction());
			}
		}
	}
	else if (const CGroup* group = dynamic_cast<const CGroup*>(primary))
	{
		switch (group->GetType())
		{
		case OptionNone:
			for (const CDefinition* definition : group->getDefinitionList())
			{
				RulesList rules = Parse(currentRules, definition);
				for (auto elem : rules)
					newRules.insert(elem);
			}
			break;
		case OptionRepetition:
			throw MyException("Grammar after preparing shouldn't contain any repetition rules.", -10);
			break;
		case OptionOptional:
			newRules = currentRules; // if is not present
			for (const CDefinition* definition : group->getDefinitionList())
			{
				RulesList rules = Parse(currentRules, definition);
				for (auto elem : rules)
					newRules.insert(elem); // if is present
			}
			break;
		default:
			throw MyException("Unknown group type", -10);
		}
	}
	else if (const CSpecial* special = dynamic_cast<const CSpecial*>(primary))
	{
		throw MyException("Special sequences not supported in parsing yet.", -1);
	}


	if (term->HasException())
		throw MyException("Exception not supported in parsing yet.", -1);
	return newRules;
}

void CParsingTable::Parse(const CSyntax & grammar)
{
	if (grammar.empty())
		return;
	rulesMap.clear();
	int nr=0;
	for (CSyntaxRule* rule : grammar)
		rulesMap[rule->GetIdentifier()].push_back(make_pair(nr++, &rule->GetDefinitionList()));

	//this->push_back(new CParserAction());
	ParseIdentifier(grammar[0]->GetIdentifier());
	//int ruleNo = 0;
	//for (const CSyntaxRule* rule : grammar)
	//{
	//	for (const CDefinition* definition : rule->GetDefinitionList())
	//	{
	//		RulesList endSymbols = Parse(RulesList{ 0 }, definition);
	//		for (auto elem : endSymbols)
	//		{
	//			(*this)[elem]->reductionRule = ruleNo;
	//		}
	//	}
	//	ruleNo++;
	//}
}

CParsingTable::~CParsingTable()
{
}
