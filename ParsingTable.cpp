#include "ParsingTable.h"
#include "SyntaxRule.h"
#include "Term.h"
#include "Definition.h"
#include "main.h"
#include "Group.h"
#include "Special.h"


CParsingTable::CParsingTable()
{
}

CParsingTable::CParsingTable(const CParsingTable & other)
{
}

CParsingTable::RulesList CParsingTable::Parse(const RulesList& currentRules, const CDefinition * definition)
{
	RulesList result;
	for (const CTerm* term : *definition)
	{
		RulesList localRules = currentRules;
		Parse(localRules, term);
		for (auto elem : localRules)
			result.insert(elem);
	}
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
			if (int gotoAction = currentAction->GetGoto(identifier) != -1)
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
			if (int shiftAction = currentAction->GetShiftAction(terminal) != -1)
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
	this->push_back(new CParserAction());
	for (const CSyntaxRule* rule : grammar)
	{
		for (const CDefinition* definition : rule->GetDefinitionList())
			Parse(RulesList{ 0 }, definition);
	}
}

CParsingTable::~CParsingTable()
{
}
