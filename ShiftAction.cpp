#include "Parser.h"
#include "ShiftAction.h"

namespace Parser
{
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
}