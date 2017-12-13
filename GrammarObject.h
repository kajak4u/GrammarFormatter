#pragma once
#include <functional>
#include <set>

namespace GrammarSymbols
{
	class CGrammarObject
	{
#ifdef DEBUG_MEMLEAKS
		static int counter;
		static std::set<CGrammarObject*> existing;
#endif
	public:
		CGrammarObject();
#ifdef DEBUG_MEMLEAKS
		CGrammarObject(const CGrammarObject& other);
		CGrammarObject(CGrammarObject&&);
		CGrammarObject& operator=(const CGrammarObject& other) = default;
		CGrammarObject& operator=(CGrammarObject&&) = default;
#endif
		virtual ~CGrammarObject();
		static void PrintCounter();
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