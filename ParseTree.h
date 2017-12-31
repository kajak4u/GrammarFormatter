#pragma once

namespace Parser
{

	class CParsingState;

	//base class representing any node in parsing tree
	class CParseTree
	{
		//parser's state associated with the state
		CParsingState* state;
	public:
		//returns associated parser's state
		CParsingState* const GetState() const;
		//sets associated parser's state
		void SetState(CParsingState* newState);
		//constructor
		CParseTree(CParsingState* state);
		//destructor
		virtual ~CParseTree();
	};

}