#pragma once

#include <iostream>

//interface for stream reading/writing
class IReadable
{
protected:
	virtual _STD istream& ReadFrom(_STD istream& is) = 0;
	virtual void WriteTo(_STD ostream& os) const = 0;
public:
	virtual ~IReadable();
	friend _STD istream& operator >> (_STD istream& is, IReadable& obj);
	friend _STD ostream& operator << (_STD ostream& os, const IReadable& obj);
};
_STD istream& operator>>(_STD istream& is, IReadable& obj);
_STD ostream& operator<<(_STD ostream& os, const IReadable& obj);
