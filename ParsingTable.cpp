#include "ParsingTable.h"
#include "Situation.h"
#include "ParsingState.h"
#include "AcceptAction.h"
#include "ShiftAction.h"
#include "ReduceAction.h"
#include "Goto.h"

using namespace std;

namespace Parser
{

	CSituations CParsingTable::Closure(const CSituations & situations)
	{
		CSituations toProcess = situations;
		CSituations processed;
		while (!toProcess.empty())
		{
			CSituation situation = *toProcess.begin();
			processed += situation;
			toProcess -= situation;
			if (CDefinedGrammarSymbol* B = dynamic_cast<CDefinedGrammarSymbol*>(DereferenceOrNull(situation.pos, *situation.def)))
			{
				auto iter = _STD next(situation.pos);
				auto beta = DereferenceOrNull(iter, *situation.def);
				auto a = situation.allowed;
				const MySet<const IDefinition*>& definitions = B->GetDefinitions();
				vector<CPrimary*> v = { beta, a };
				MySet<CTerminal*, CompareObjects<CTerminal>> terminals = GetFirstFrom(iter, situation.def->end());//GetFirstFrom(_STD next(v.begin(), beta == nullptr), v.end());
				if (terminals.Contains(nullptr))
				{
					terminals -= nullptr;
					terminals += a;
				}
				for (auto& definition : definitions)
				{
					CSituation newSituation = CSituation(B, dynamic_cast<const CShortDefinition*>(definition));
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

	CSituations CParsingTable::Goto(const CSituations & I, const CPrimary * symbol)
	{
		CSituations J;
		for (const CSituation& situation : I)
			if (symbol == nullptr)
			{
				if (DereferenceOrNull(situation.pos, *situation.def) == nullptr)
					J.insert(situation.nextPos());
			}
			else if (symbol->Equals(DereferenceOrNull(situation.pos, *situation.def)))
				J.insert(situation.nextPos());
		return Closure(J);
	}

	CParsingTable::CParsingTable(const GrammarSymbols::CSyntax & grammar)
	{
		auto startSymbol = grammar.GetStartSymbol();
		auto& definitions = startSymbol->GetDefinitions();
		CSituations startSituations;
		CTerminal* endingTerminal = CTerminal::Unique();
		for (auto& elem : definitions)
		{
			if (const CShortDefinition* def = dynamic_cast<const CShortDefinition*>(elem))
				startSituations.insert(CSituation(startSymbol, def, endingTerminal));
		}
		this->push_back(new CParsingState(new CSituations(_STD move(Closure(startSituations)))));

		for (unsigned i = 0; i < this->size(); ++i)
		{
			auto& state = *(*this)[i];
			auto& situations = *state.GetSituations();
			auto cmp = [](const CPrimary* const a, const CPrimary* const b) {return a->Compare(b) < 0; };
			MySet<CPrimary*, std::function<bool(const CPrimary*, const CPrimary*)>> symbols(cmp);
			for (const CSituation& situation : situations)
				if (situation.pos == situation.def->end())
				{
					if (true || !situation.def->empty())
					{
						if (startSymbol->Equals(situation.result) && endingTerminal->Equals(situation.allowed))
							state.AddAction(endingTerminal, new CAcceptAction());
						else
							state.AddAction(situation.allowed, new CReduceAction(situation.result, situation.def));
					}
				}
				else
					symbols += *situation.pos;
			for (CPrimary* const& symbol : symbols)
			{
				CSituations newSituations = Goto(situations, symbol);
				if (newSituations.empty())
					continue;
				CParsingState* newState = this->AddOrGet(&newSituations);
				if (const CDefinedGrammarSymbol* identifier = dynamic_cast<const CDefinedGrammarSymbol*>(symbol))
					state.AddGoto(identifier, new CGoto(newState));
				else if (const CTerminal* terminal = dynamic_cast<const CTerminal*>(symbol))
					state.AddAction(terminal, new CShiftAction(newState));
			}
		}
	}

	CParsingTable::~CParsingTable()
	{
		for (auto& state : *this)
			delete state;
	}

	CParsingState * CParsingTable::AddOrGet(CSituations * situations)
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
			if (compareCSituations(elem->GetSituations(), situations))
				return elem;
#ifdef _DEBUG
		push_back(new CParsingState(size(), new CSituations(std::move(*situations))));
#else
		push_back(new CParsingState(new CSituations(std::move(*situations))));
#endif
		return back();
	}

	std::ostream & operator<<(std::ostream & os, const CParsingTable & table)
	{

		int i = -1;
		for (auto& situations : table)
		{
			os << "## i" << (++i) << endl;
			for (auto& situation : *situations->GetSituations())
				os << "  " << situation << endl;
		}
		return os;
	}
	
}