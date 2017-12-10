#include "Parser.h"

#include "Syntax.h"
#include "main.h"
using namespace std;

CSituations CParser::Closure(const CSituations & situations)
{
	CSituations toProcess = situations;
	CSituations processed;
	while (!toProcess.empty())
	{
		CSituation situation = *toProcess.begin();
		processed += situation;
		toProcess -= situation;
		if (CMetaIdentifier* B = dynamic_cast<CMetaIdentifier*>(DereferenceOrNull(situation.pos, *situation.def)))
		{
			auto beta = DereferenceOrNull(_STD next(situation.pos), *situation.def);
			auto a = situation.allowed;
			const MySet<const IDefinition*>& definitions = B->GetDefinitions();
			vector<CPrimary*> v = { beta, a };
			MySet<CTerminal*> terminals = GetFirstFrom(_STD next(v.begin(), beta == nullptr), v.end());
			for (auto& definition : definitions)
			{
				CSituation newSituation(B, dynamic_cast<const CShortDefinition*>(definition));
				for (auto& terminal : terminals)
				{
					newSituation.allowed = terminal;
					if (!processed.Contains(newSituation))
						toProcess += newSituation;
				}
			}
		}
	}
	return processed;
}

CSituations CParser::Goto(const CSituations & I, const CPrimary * symbol)
{
	CSituations J;
	for (const CSituation& situation : I)
		if(symbol->Equals(DereferenceOrNull(situation.pos, *situation.def)))
			J.insert(situation.nextPos());
	return Closure(J);
}

CParser::CParser()
{
}

#include <map>


class CAction
{
public:
	virtual void Perform(CParser& parser) = 0;
};
class CParsingState;
class CGoto
{
	CParsingState* newState;
public:
	CGoto(CParsingState* newState)
		: newState(newState)
	{}
	void Perform(CParser& parser)
	{
//		parser.ChangeStateTo(newState);
	}
};


class CParsingState
{
public:
	CSituations* situations;
	_STD map<const CTerminal*, CAction*> actions;
	_STD map<const CMetaIdentifier*, CGoto*> gotos;
	CParsingState(CSituations* situations)
		: situations(situations)
	{}
};

class CShiftAction : public CAction
{
	CParsingState* newState;
public:
	CShiftAction(CParsingState* newState)
		: newState(newState)
	{}
	// Inherited via CAction
	virtual void Perform(CParser & parser) override
	{
//		parser.AddSymbolToStack();
//		parser.ChangeStateTo(newState);
	}
};
class CAcceptAction : public CAction
{
public:
	// Inherited via CAction
	virtual void Perform(CParser & parser) override
	{
//		parser.Accept();
	}
};
class CReduceAction : public CAction
{
	const CMetaIdentifier* result;
	const CShortDefinition* definition;
public:
	CReduceAction(const CMetaIdentifier* result, const CShortDefinition* definition)
		: result(result), definition(definition)
	{}
	// Inherited via CAction
	virtual void Perform(CParser & parser) override
	{
//		parser.ReduceFrom(definition);
//		parser.AddToStack(result);
	}
};

class CParsingTable : public vector<CParsingState*>
{
public:
	CParsingState* AddOrGet(CSituations* situations)
	{
		auto compareCSituations = [](const CSituations* s1, const CSituations* s2)
		{
			if (s1->size() != s2->size())
				return false;
			for (auto i1 = s1->begin(), i2 = s2->begin(); i1 != s1->end(); ++i1, ++i2)
				if (!(*i1 == *i2))
					return false;
			return true;
		};
		for (auto& elem : *this)
			if (compareCSituations(elem->situations, situations))
				return elem;
		
		push_back(new CParsingState(new CSituations(std::move(*situations))));
		return back();
	}
};

void CParser::CreateParsingTable(const CSyntax & grammar)
{
	auto& startSymbol = grammar.GetStartSymbol();
	auto& definitions = startSymbol.GetDefinitions();
	CSituations startSituations;
	CTerminal* endingTerminal = CTerminal::CreateUnique();
	for (auto& elem : definitions)
	{
		if (const CShortDefinition* def = dynamic_cast<const CShortDefinition*>(elem))
			startSituations.insert(CSituation(&startSymbol, def, endingTerminal));
	}
	CParsingTable sets;
	sets.push_back(new CParsingState(new CSituations(_STD move(Closure(startSituations)))));

	for (unsigned i=0;i<sets.size();++i)
	{
		auto& state = *sets[i];
		auto& situations = *state.situations;
		MySet<CPrimary*> symbols;
		for (const CSituation& situation : situations)
			if (situation.pos == situation.def->end())
			{
				if (startSymbol.Equals(situation.result) && endingTerminal->Equals(situation.allowed))
					state.actions[endingTerminal] = new CAcceptAction();
				else
					state.actions[situation.allowed] = new CReduceAction(situation.result, situation.def);
			}
			else
				symbols += *situation.pos;
		for (CPrimary* const& symbol : symbols)
		{
			CSituations newSituations = Goto(situations, symbol);
			if (newSituations.empty())
				continue;
			CParsingState* newState = sets.AddOrGet(&newSituations);
			if (const CMetaIdentifier* identifier = dynamic_cast<const CMetaIdentifier*>(symbol))
				state.gotos[identifier] = new CGoto(newState);
			else if (const CTerminal* terminal = dynamic_cast<const CTerminal*>(symbol))
				state.actions[terminal] = new CShiftAction(newState);
		}
	}
	cerr << "Zbiory sytuacji:" << endl;
	int i = -1;
	for (auto& situations : sets)
	{
		cerr << "## i" << (++i) << endl;
		for (auto& situation : *situations->situations)
		{
			cerr << "  " << situation << endl;
		}
	}

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
		(*iter)->WriteTo(os);
	}
	if (situation.pos == situation.def->end())
		os << " *";
	os << ", " << *situation.allowed;
	return os;
}
