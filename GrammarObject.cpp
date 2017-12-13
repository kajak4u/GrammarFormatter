#include "GrammarObject.h"
#include <iostream>

namespace GrammarSymbols
{
#ifdef DEBUG_MEMLEAKS
	int CGrammarObject::counter = 0;
	std::set<CGrammarObject*> CGrammarObject::existing;
	CGrammarObject::CGrammarObject()
	{
		existing.insert(this);
		++counter;
		std::cerr << "Created grammar object, currently " << counter << " exist" << std::endl;
	}
	CGrammarObject::CGrammarObject(const CGrammarObject & other)
		: CGrammarObject()
	{
	}
	CGrammarObject::CGrammarObject(CGrammarObject &&)
		: CGrammarObject()
	{
	}
	CGrammarObject::~CGrammarObject()
	{
		if(existing.find(this)!=existing.end())
			existing.erase(this);
		else
		{
			std::cerr << "deleting object not in memory!" << std::endl;
		}
		--counter;
		std::cerr << "Deleted grammar object, currently " << counter << " exist" << std::endl;
	}
	void CGrammarObject::PrintCounter()
	{
		std::cerr << "Currently " << counter << " objects exist" << std::endl;
	}
#else
	CGrammarObject::CGrammarObject()
	{}
	CGrammarObject::~CGrammarObject()
	{}
#endif
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