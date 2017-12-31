#pragma once


namespace Parser
{
	class CParser;

	//abstract class representing parser's action 
	class CAction
	{
	public:
		//perform the action on given parser
		virtual void Perform(CParser& parser) = 0;
	};
}