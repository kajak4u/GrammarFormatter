#include "ParserAction.h"
using namespace std;

void CParserAction::AddGoto(const CMetaIdentifier * id, GotoRule nextRule)
{
	if (!gotos)
		gotos = new unordered_map<const CMetaIdentifier*, GotoRule>{ {id, nextRule} };
	else
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
		actions = new unordered_map<const CTerminal*, GotoRule>{ { id, nextRule } };
	else
		actions->insert(make_pair(id, nextRule));
}

CParserAction::SelectionRule CParserAction::GetShiftAction(const CTerminal * id) const
{
	if (!actions)
		return -1;
	auto iter = actions->find(id);
	return iter == actions->end() ? -1 : iter->second;
}
