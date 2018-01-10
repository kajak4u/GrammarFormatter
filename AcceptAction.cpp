#include "AcceptAction.h"
#include "Parser.h"

namespace Parser
{
	void CAcceptAction::Perform(CParser & parser)
	{
#ifdef DEBUG_PARSING
		std::cerr << "Accept " << std::endl;
#endif
		parser.Accept();
	}
}