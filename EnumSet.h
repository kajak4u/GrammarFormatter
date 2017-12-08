#pragma once

#include <set>

template<typename ENUM> class CEnumSet : private std::set<ENUM>
{
public:
	bool inline operator&(const ENUM& elem)
	{
		return Contains(elem);
	}
	bool inline Contains(const ENUM& elem)
	{
		return find(elem) != end();
	}
	CEnumSet<ENUM>& operator|=(const ENUM &elem)
	{
		this->insert(elem);
		return *this;
	}
};