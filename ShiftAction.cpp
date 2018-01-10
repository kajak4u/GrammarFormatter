#include "Parser.h"
#include "ShiftAction.h"
#include "ParsingState.h"

namespace Parser
{
	CShiftAction::CShiftAction(CParsingState * newState)
		: newState(newState)
	{}

	void CShiftAction::Perform(CParser & parser)
	{
#ifdef DEBUG_PARSING
#ifdef _DEBUG
		std::cerr << "Shift " << newState->id << std::endl;
#else
		std::cerr << "Shift " << std::endl;
#endif
#endif
		parser.AddStateToStack(newState);
	}
}