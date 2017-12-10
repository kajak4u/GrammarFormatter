#include "Parser.h"

#include "Syntax.h"
#include "main.h"
using namespace std;

CParser::CSituations CParser::Closure(const CSituations & situations)
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
			auto beta = DereferenceOrNull(std::next(situation.pos), *situation.def);
			auto a = situation.allowed;
			const MySet<const IDefinition*>& definitions = B->GetDefinitions();
			vector<CPrimary*> v = { beta, a };
			MySet<CTerminal*> terminals = GetFirstFrom(std::next(v.begin(), beta == nullptr), v.end());
			for (auto& definition : definitions)
			{
				CSituation newSituation(dynamic_cast<const CShortDefinition*>(definition));
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

CParser::CSituations CParser::Goto(const CSituations & I, const CPrimary * symbol)
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

void CParser::Parse(const CSyntax & grammar)
{
	auto& definitions = grammar.GetStartSymbol().GetDefinitions();
	CSituations startSituations;
	CTerminal* endingTerminal = CTerminal::CreateUnique();
	for (auto& elem : definitions)
	{
		if (const CShortDefinition* def = dynamic_cast<const CShortDefinition*>(elem))
			startSituations.insert(CSituation(def, endingTerminal));
	}
	vector<CSituations*> sets;
	sets.push_back(new CSituations(std::move(Closure(startSituations))));
	auto compareCSituations = [](const CSituations* s1, const CSituations* s2)
	{
		if (s1->size() != s2->size())
			return false;
		for (auto i1 = s1->begin(), i2 = s2->begin(); i1 != s1->end(); ++i1, ++i2)
			if (!(*i1 == *i2))
				return false;
		return true;
	};
	auto Contains = [&compareCSituations](const vector<CSituations*>& v, const CSituations* item)
	{
		for (const CSituations* const& elem : v)
			if (compareCSituations(elem, item))
				return true;
		return false;
	};

	for (unsigned i=0;i<sets.size();++i)
	{
		auto situations = sets[i];
		MySet<CPrimary*> symbols;
		for (const CSituation& situation : *situations)
			if (situation.pos != situation.def->end())
				symbols += *situation.pos;
		for (CPrimary* const& symbol : symbols)
		{
			CSituations newSituations = Goto(*situations, symbol);
			if (!newSituations.empty() && !Contains(sets, &newSituations))
				sets.push_back(new CSituations(std::move(newSituations)));
		}
	}
	cerr << "Zbiory sytuacji:" << endl;
	int i = -1;
	for (auto& situations : sets)
	{
		cerr << "## i" << (++i) << endl;
		for (auto& situation : *situations)
		{
			cerr << "  " << situation << endl;
		}
	}

}


CParser::~CParser()
{
}

std::ostream & operator<<(std::ostream & os, const CSituation & situation)
{
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
