#pragma once
#include "Action.h"

namespace Parser
{
	class CParsingState;

	//represents parser's action 'SHIFT [state]'
	class CShiftAction : public CAction
	{
		//next parser's state
		CParsingState* newState;
	public:
		CShiftAction(CParsingState* newState);
		virtual void Perform(CParser & parser) override;
	};
}