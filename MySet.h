#pragma once

#include <set>
#include <algorithm>

template<typename _T> class MySet : public _STD set<_T>
{
public:
	using Self = MySet<_T>;
	bool Contains(const _T& item)
	{
		return find(item) != end();
	}
	bool IsSubsetOf(const Self& superset)
	{
		return _STD includes(superset.begin(), superset.end(), begin(), end());
	}

	Self& operator+=(const _T& item)
	{ 
		insert(item); 
		return *this; 
	}
	Self& operator+=(const Self& other)
	{
		insert(other.begin(), other.end());
		return *this;
	}
	Self& operator-=(const _T& elem)
	{
		erase(elem);
		return *this;
	}

// short operators - defining oper '+' having oper '+=' etc.
#define OPER_ELEM(op) Self operator op (const _T&   elem)  const {Self obj = *this; obj op ## = elem;  return obj;}
#define OPER_SET(op)  Self operator op (const Self& other) const {Self obj = *this; obj op ## = other; return obj;}

	OPER_ELEM(+);
	OPER_ELEM(-);
	OPER_SET(+);
	OPER_SET(-);
};