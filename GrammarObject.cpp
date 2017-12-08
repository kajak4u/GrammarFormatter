#include "GrammarObject.h"

void CGrammarObject::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	if (condition(this))
		action(this);
}

void CGrammarObject::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	if (condition(this))
		action(this);
}