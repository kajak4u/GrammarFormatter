#pragma once

namespace Parser
{
	class CParser;
	class CParsingState;

	//represents parser's state 'goto [state]'
	class CGoto
	{
		//next parser's state
		CParsingState* newState;
	public:
		CGoto(CParsingState* newState);
		void Perform(CParser& parser);
	};
}