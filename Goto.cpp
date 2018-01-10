#include "Goto.h"
#include "Parser.h"
#include "ParsingState.h"

namespace Parser
{
	CGoto::CGoto(CParsingState* newState)
		: newState(newState)
	{}

	void CGoto::Perform(CParser & parser)
	{
#ifdef DEBUG_PARSING
#ifdef _DEBUG
		std::cerr << "Goto " << newState->id << std::endl;
#else
		std::cerr << "Goto " << std::endl;
#endif
#endif
		parser.ChangeStateTo(newState);
	}
}