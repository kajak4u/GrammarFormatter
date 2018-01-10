#include "ReduceAction.h"
#include "Parser.h"
#include "DefinedGrammarSymbol.h"
#include "ShortDefinition.h"

namespace Parser
{
	CReduceAction::CReduceAction(const CDefinedGrammarSymbol * result, const CShortDefinition * definition)
		: result(result), definition(definition)
	{}

	void CReduceAction::Perform(CParser & parser)
	{
#ifdef DEBUG_PARSING
		std::cerr << "Reduce from " << *result << " = " << *definition << std::endl;
#endif
		parser.Reduce(result, definition);
	}
}