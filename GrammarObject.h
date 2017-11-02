#pragma once
#include <functional>

class CGrammarObject
{
public:
	virtual void ForEach(
		std::function<bool(const CGrammarObject*)> condition, 
		std::function<void(const CGrammarObject*)> action
	) const;
};