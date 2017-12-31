#pragma once
#include <functional>
#include <set>

namespace GrammarSymbols
{
	class CGrammarObject;

	//definitions to make ForEach() declarations more readable
	using GrammarObjectPredicate = _STD function< bool(const CGrammarObject*)>;
	using GrammarObjectAction = _STD function< void(CGrammarObject*)>;
	using GrammarObjectConstAction = _STD function< void(const CGrammarObject*)>;

	//base class for all EBNF grammar's classes
	class CGrammarObject
	{
#ifdef DEBUG_MEMLEAKS
		//in memleaks mode - counts existing objects to detect memory leaks
		static int counter;
		//set of existing objects
		static std::set<CGrammarObject*> existing;
#endif
	public:
		//constructor
		CGrammarObject();
#ifdef DEBUG_MEMLEAKS
		//override copy constructor to increase counter
		CGrammarObject(const CGrammarObject& other);
		//override move constructor to increase counter
		CGrammarObject(CGrammarObject&&);
		//leave assign operator default
		CGrammarObject& operator=(const CGrammarObject& other) = default;
		//leave assign move operator default
		CGrammarObject& operator=(CGrammarObject&&) = default;
		//print counter's value
		static void PrintCounter();
#endif
		//destructor
		virtual ~CGrammarObject();

		//for each object in grammar's subtree that matches the [condition], perform [action] on it
		virtual void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action);
		//for each object in grammar's subtree that matches the [condition], perform [action] on it - const version
		virtual void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const;
	};
}