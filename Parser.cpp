#include "Parser.h"

#include "Syntax.h"
#include "main.h"
using namespace std;


CParser::CSituations CParser::Closure(const MySet<CSituation>& situations)
{
	CSituations toProcess = situations;
	CSituations processed;
	while (!toProcess.empty())
	{
		CSituation situation = *toProcess.begin();
		processed += situation;
		toProcess -= situation;
		if (CMetaIdentifier* B = dynamic_cast<CMetaIdentifier*>(*situation.pos))
		{
			auto beta = DereferenceOrNull(std::next(situation.pos), *situation.def);
			auto a = situation.allowed;
			const MySet<const IDefinition*>& definitions = B->GetDefinitions();
			vector<CPrimary*> v = {beta, a};
			MySet<CTerminal*> terminals = GetFirstFrom(std::next(v.begin(), beta==nullptr), v.end());
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

CParser::CParser()
{
}

void CParser::Parse(const CSyntax & grammar)
{
	auto& definitions = grammar.GetStartSymbol().GetDefinitions();
	CSituations situations;
	for (auto& elem : definitions)
	{
		if (const CShortDefinition* def = dynamic_cast<const CShortDefinition*>(elem))
		{
			auto& allowed = next(def->begin());
			if (allowed == def->end())
				throw MyException("Incorrect start auto-definition\n" __FILE__, __LINE__);
			situations.insert(CSituation(def, dynamic_cast<CTerminal*>(*allowed)));
		}
	}
	auto& i0 = Closure(situations);
	cout << "closure contains " << i0.size() << " elements." << endl;
}


CParser::~CParser()
{
}
