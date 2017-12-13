#include "ParsingTable.h"
#include "main.h"
#include "ShortDefinition.h"

using namespace std;


CSituations CParsingTable::Closure(const CSituations & situations)
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
			MySet<CTerminal*, CompareObjects<CTerminal>> terminals = GetFirstFrom(_STD next(v.begin(), beta == nullptr), v.end());
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

	for (unsigned i = 0; i<this->size(); ++i)
	{
		auto& state = *(*this)[i];
		auto& situations = *state.situations;
		auto cmp = [](const CPrimary* const a, const CPrimary* const b) {return a->Compare(b) < 0; };
		MySet<CPrimary*, std::function<bool(const CPrimary*, const CPrimary*)>> symbols(cmp);
		for (const CSituation& situation : situations)
			if (situation.pos == situation.def->end())
			{
				if (true || !situation.def->empty())
				{
					if (startSymbol->Equals(situation.result) && endingTerminal->Equals(situation.allowed))
						state.actions[endingTerminal] = new CAcceptAction();
					else
						state.actions[situation.allowed] = new CReduceAction(situation.result, situation.def);
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
			if (const CMetaIdentifier* identifier = dynamic_cast<const CMetaIdentifier*>(symbol))
				state.gotos[identifier] = new CGoto(newState);
			else if (const CTerminal* terminal = dynamic_cast<const CTerminal*>(symbol))
				state.actions[terminal] = new CShiftAction(newState);
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
		if (compareCSituations(elem->situations, situations))
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
		for (auto& situation : *situations->situations)
			os << "  " << situation << endl;
	}
	return os;
}

CGoto::CGoto(CParsingState* newState)
	: newState(newState)
{}

void CGoto::Perform(CParser & parser)
{
#ifdef DEBUG_PARSING
#ifdef _DEBUG
	cerr << "Goto " << newState->id << endl;
#else
	cerr << "Goto " << endl;
#endif
#endif
	parser.ChangeStateTo(newState);
}

CShiftAction::CShiftAction(CParsingState * newState)
	: newState(newState)
{}

void CShiftAction::Perform(CParser & parser)
{
#ifdef DEBUG_PARSING
#ifdef _DEBUG
	cerr << "Shift " << newState->id << endl;
#else
	cerr << "Shift " << endl;
#endif
#endif
	parser.AddStateToStack(newState);
}

void CAcceptAction::Perform(CParser & parser)
{
#ifdef DEBUG_PARSING
	cerr << "Accept " << endl;
#endif
	parser.Accept();
}

CReduceAction::CReduceAction(const CMetaIdentifier * result, const CShortDefinition * definition)
	: result(result), definition(definition)
{}

void CReduceAction::Perform(CParser & parser)
{
#ifdef DEBUG_PARSING
	cerr << "Reduce from " << *result << " = ";
	definition->WriteTo(cerr);
	cerr << endl;
#endif
	parser.Reduce(result, definition);
}

CParsingState::~CParsingState()
{
	delete situations;
	// nie usuwamy [first], bo one byly kopiowane z gramatyki
	for (auto& keyVal : actions)
	{
		delete keyVal.second;
	}
	for (auto& keyVal : gotos)
	{
		delete keyVal.second;
	}
}
