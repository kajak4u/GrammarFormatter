#pragma once

#include <iostream>

//interface for stream reading/writing
class IReadable
{
public:
	virtual _STD istream& ReadFrom(_STD istream& is) = 0;
	virtual void WriteTo(_STD ostream& os) const = 0;
	virtual ~IReadable();
};
_STD istream& operator<<(_STD istream& is, IReadable* obj);
_STD ostream& operator>>(_STD ostream& os, const IReadable* obj);