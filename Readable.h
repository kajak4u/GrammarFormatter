#pragma once

#include <iostream>

class IReadable
{
public:
	virtual _STD istream& ReadFrom(_STD istream& is) = 0;
	virtual void WriteTo(_STD ostream& os) const = 0;
	virtual ~IReadable();
};
_STD istream& operator<<(_STD istream& is, IReadable& obj);