#pragma once

#include <iostream>

//interface for stream reading/writing
class IReadable
{
public:
	virtual ~IReadable();
	virtual _STD istream& ReadFrom(_STD istream& is) = 0;
	virtual void WriteTo(_STD ostream& os) const = 0;
};
_STD istream& operator>>(_STD istream& is, IReadable& obj);
_STD ostream& operator<<(_STD ostream& os, const IReadable& obj);
