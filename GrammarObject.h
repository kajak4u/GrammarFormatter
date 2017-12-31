#pragma once
#include <functional>
#include <set>

namespace GrammarSymbols
{
	class CGrammarObject;

	using GrammarObjectPredicate = _STD function< bool(const CGrammarObject*)>;
	using GrammarObjectAction = _STD function< void(CGrammarObject*)>;
	using GrammarObjectConstAction = _STD function< void(const CGrammarObject*)>;

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
		virtual void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action);
		virtual void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const;
	};
}