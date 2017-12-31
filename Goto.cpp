#include "Goto.h"
#include "Parser.h"

namespace Parser
{
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
}