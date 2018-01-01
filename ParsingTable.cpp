#include "AcceptAction.h"
#include "Goto.h"
#include "ParsingState.h"
#include "ParsingTable.h"
#include "ReduceAction.h"
#include "ShiftAction.h"
#include "Syntax.h"
#include "Terminal.h"

using namespace std;

namespace Parser
{

	CSituations CParsingTable::Closure(const CSituations & situations)
	{
		CSituations toProcess = situations;
		CSituations processed;
		//for each situation
		while (!toProcess.empty())
		{
			CSituation situation = *toProcess.begin();
			//mark as processed
			processed += situation;
			toProcess -= situation;
			//if [pos] points at symbol, add its derivations to closure (with pos at the beginning)
			if (CDefinedGrammarSymbol* B = dynamic_cast<CDefinedGrammarSymbol*>(DereferenceOrNull(situation.pos, *situation.def)))
			{
				auto iter = _STD next(situation.pos);
				MySet<CTerminal*, CompareObjects<CTerminal>> terminals = GetFirstFrom(iter, situation.def->end());
				//[allowed] will follow the definition, so if remaining part of definition can be derived from empty symbol, 
				//  add [allowed] instead of empty for it will be the next terminal
				if (terminals.Contains(nullptr))
				{
					terminals -= nullptr;
					terminals += situation.allowed;
				}
				//for each definition and each allowed terminal, add it to processing queue
				for (auto& definition : B->GetDefinitions())
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
		//find every situation with [pos] at [symbol], then move theirs [pos] forward
		CSituations J;
		if (symbol == nullptr)
		{
			for (const CSituation& situation : I)
				if (DereferenceOrNull(situation.pos, *situation.def) == nullptr)
					J.insert(situation.nextPos());
		}
		else
			for (const CSituation& situation : I)
				if (symbol->Equals(DereferenceOrNull(situation.pos, *situation.def)))
					J.insert(situation.nextPos());
		//return Closure for the new situation set
		return Closure(J);
	}

	CParsingTable::CParsingTable(const GrammarSymbols::CSyntax & grammar)
	{
		//start situations are made of [start symbol]'s rules' Closure
		CSituations startSituations;
		auto startSymbol = grammar.GetStartSymbol();
		CTerminal* endingTerminal = CTerminal::Unique();
		for (auto& elem : startSymbol->GetDefinitions())
		{
			if (const CShortDefinition* def = dynamic_cast<const CShortDefinition*>(elem))
				startSituations.insert(CSituation(startSymbol, def, endingTerminal));
		}
#ifdef _DEBUG
		this->push_back(new CParsingState(size(), new CSituations(_STD move(Closure(startSituations)))));
#else
		this->push_back(new CParsingState(new CSituations(_STD move(Closure(startSituations)))));
#endif

		//[i] points to state being actually processed
		//cannot use iterators due to adding new items to container
		for (unsigned i = 0; i < this->size(); ++i)
		{
			auto& state = *(*this)[i];
			auto& situations = *state.GetSituations();
			auto cmp = [](const CPrimary* const a, const CPrimary* const b) {return a->Compare(b) < 0; };
			MySet<CPrimary*, std::function<bool(const CPrimary*, const CPrimary*)>> symbols(cmp);
			for (const CSituation& situation : situations)
				//if situation's [pos] points at end, then perform either 'REDUCE' or 'ACCEPT' (accept when reducing to start symbol)
				if (situation.pos == situation.def->end())
				{
					if (startSymbol->Equals(situation.result) && endingTerminal->Equals(situation.allowed))
						state.AddAction(endingTerminal, new CAcceptAction());
					else
						state.AddAction(situation.allowed, new CReduceAction(situation.result, situation.def));
				}
				//extend list of symbols that can occur next (if symbol is not there yet)
				else
					symbols += *situation.pos;
			//for each following symbol with non-empty situation set, add either 'GOTO' (for symbol) or 'SHIFT' (for terminal)
			for (CPrimary* const& symbol : symbols)
			{
				CSituations newSituations = Goto(situations, symbol);
				if (newSituations.empty())
					continue;
				//do not create a new state if there is already the same in table
				CParsingState* newState = this->AddOrGet(&newSituations);
				if (const CDefinedGrammarSymbol* identifier = dynamic_cast<const CDefinedGrammarSymbol*>(symbol))
					state.AddGoto(identifier, new CGoto(newState));
				else if (const CTerminal* terminal = dynamic_cast<const CTerminal*>(symbol))
					state.AddAction(terminal, new CShiftAction(newState));
			}
		}
	}

	CParsingTable::CParsingTable(CParsingTable && other)
		: vector<CParsingState*>(std::move(other))
	{
		other.clear();
	}

	CParsingTable::~CParsingTable()
	{
		for (auto& state : *this)
			delete state;
	}

	CParsingState * CParsingTable::AddOrGet(CSituations * situations)
	{
		//comparing situations - firstly compare size, then in loop compare each element
		auto compareCSituations = [](const CSituations* s1, const CSituations* s2)
		{
			if (s1->size() != s2->size())
				return false;
			for (auto i1 = s1->begin(), i2 = s2->begin(); i1 != s1->end(); ++i1, ++i2)
				if (!(*i1 == *i2))
					return false;
			return true;
		};
		//if a state is already there, return it instead of creating a new one
		for (auto& elem : *this)
			if (compareCSituations(elem->GetSituations(), situations))
				return elem;
#ifdef _DEBUG
		//in debug mode, initialize state's [id] field
		push_back(new CParsingState(size(), new CSituations(std::move(*situations))));
#else
		//in release mode, there is no such field
		push_back(new CParsingState(new CSituations(std::move(*situations))));
#endif
		//return new state
		return back();
	}

	std::ostream & operator<<(std::ostream & os, const CParsingTable & table)
	{
		//print each situation set with its record number
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