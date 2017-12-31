#include "ReduceAction.h"
#include "Parser.h"

namespace Parser
{
	CReduceAction::CReduceAction(const CDefinedGrammarSymbol * result, const CShortDefinition * definition)
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
}