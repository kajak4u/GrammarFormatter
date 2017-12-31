#pragma once

#include <set>
#include <algorithm>

//custom MySet class extending std::set with +/-/+=/-= operators and subset checking
template<typename _T, class _Pr = std::less<_T>> class MySet : public _STD set<_T, _Pr>
{
public:
	//constructor
	MySet() : set() {}
	//constructor with comparing predicate
	MySet(_Pr compare) : set(compare) {}
	using Self = MySet<_T, _Pr>;
	//returns whether set contains given element
	bool Contains(const _T& item)
	{
		return find(item) != end();
	}
	//returns whether this set is subset of given set
	bool IsSubsetOf(const Self& superset)
	{
		return _STD includes(superset.begin(), superset.end(), begin(), end(), _Pr());
	}
	//insert element to set
	Self& operator+=(const _T& item)
	{ 
		insert(item); 
		return *this; 
	}
	//join another set
	Self& operator+=(const Self& other)
	{
		insert(other.begin(), other.end());
		return *this;
	}
	//remove single element
	Self& operator-=(const _T& elem)
	{
		erase(elem);
		return *this;
	}

// short operators - defining oper '+' having oper '+=' etc. for both: (MySet oper MySet) and (MySet oper Elem)
#define OPER_ELEM(op) Self operator op (const _T&   elem)  const {Self obj = *this; obj op ## = elem;  return obj;}
#define OPER_SET(op)  Self operator op (const Self& other) const {Self obj = *this; obj op ## = other; return obj;}

	OPER_ELEM(+);
	OPER_ELEM(-);
	OPER_SET(+);
};