#pragma once
#include "Action.h"

namespace Parser
{
	//represents parser's action 'ACCEPT'
	class CAcceptAction : public CAction
	{
	public:
		virtual void Perform(CParser & parser) override;
	};
}
