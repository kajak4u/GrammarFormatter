#include "GrammarObject.h"

namespace GrammarSymbols
{
	void CGrammarObject::ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(const CGrammarObject*)> action) const
	{
		if (condition(this))
			action(this);
	}

	void CGrammarObject::ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(CGrammarObject*)> action)
	{
		if (condition(this))
			action(this);
	}
}