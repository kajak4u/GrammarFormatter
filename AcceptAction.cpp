#include "AcceptAction.h"
#include "Parser.h"

namespace Parser
{
	void CAcceptAction::Perform(CParser & parser)
	{
#ifdef DEBUG_PARSING
		cerr << "Accept " << endl;
#endif
		parser.Accept();
	}
}