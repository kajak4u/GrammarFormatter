#include "ParserAction.h"
using namespace std;

void CParserAction::AddGoto(const CMetaIdentifier * id, GotoRule nextRule)
{
	if (!gotos)
	{
		auto compare = [](const CMetaIdentifier* a, const CMetaIdentifier* b) {return a->GetName() < b->GetName(); };
		gotos = new map<const CMetaIdentifier*, GotoRule, CMetaIdentifier::ComparePointers>(compare);
	}
	if (gotos->find(id) != gotos->end())
		cout << "Warning - symbol " << *id << " already exists, trying to change goto from " << (*gotos)[id] << " to " << nextRule << endl;
	gotos->insert(make_pair(id, nextRule));
}

CParserAction::GotoRule CParserAction::GetGoto(const CMetaIdentifier * id) const
{
	if (!gotos)
		return -1;
	auto iter = gotos->find(id);
	return iter == gotos->end() ? -1 : iter->second;
}

void CParserAction::AddShiftAction(const CTerminal * id, SelectionRule nextRule)
{
	if (!actions)
	{
		auto compare = [](const CTerminal* a, const CTerminal* b) {return a->GetValue() < b->GetValue(); };
		actions = new map<const CTerminal*, GotoRule, CTerminal::ComparePointers>(compare);
	}
	if (actions->find(id) != actions->end())
		cout << "Warning - symbol " << *id << " already exists, trying to change action from " << (*actions)[id] << " to " << nextRule << endl;
	actions->insert(make_pair(id, nextRule));
}

CParserAction::SelectionRule CParserAction::GetShiftAction(const CTerminal * id) const
{
	if (!actions)
		return -1;
	auto iter = actions->find(id);
	return iter == actions->end() ? -1 : iter->second;
}
