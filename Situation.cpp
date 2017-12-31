#include "Situation.h"
#include "Terminal.h"
#include "DefinedGrammarSymbol.h"

namespace Parser
{
	CSituation::CSituation(const CDefinedGrammarSymbol* result, const CShortDefinition* def, CShortDefinition::const_iterator pos, CTerminal* allowed)
		: result(result), def(def), pos(pos), allowed(allowed)
	{}

	CSituation::CSituation(const CDefinedGrammarSymbol* result, const CShortDefinition* def, CTerminal* allowed)
		: CSituation(result, def, def->begin(), allowed)
	{}

	CSituation CSituation::nextPos() const
	{
		return CSituation(result, def, _STD next(pos), allowed);
	}

	bool CSituation::operator<(const CSituation& other) const
	{
		return def != other.def ? def < other.def
			: pos != other.pos ? pos < other.pos
			: *allowed < *other.allowed;
	}

	bool CSituation::operator==(const CSituation& other) const
	{
		return def == other.def
			&& pos == other.pos
			&& !(*allowed < *other.allowed || *other.allowed < *allowed);
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

}