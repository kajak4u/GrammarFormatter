#pragma once
#include <functional>

namespace GrammarSymbols
{
	class CGrammarObject
	{
	public:
		virtual void ForEach(
			_STD function<bool(const CGrammarObject*)> condition,
			_STD function<void(const CGrammarObject*)> action
		) const;
		virtual void ForEach(
			_STD function<bool(const CGrammarObject*)> condition,
			_STD function<void(CGrammarObject*)> action
		);
	};
}