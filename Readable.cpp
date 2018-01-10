#include "Readable.h"

IReadable::~IReadable() {}

_STD istream& operator>>(_STD istream& is, IReadable& obj)
{
	return obj.ReadFrom(is);
}

_STD ostream& operator<<(_STD ostream& os, const IReadable& obj)
{
	obj.WriteTo(os);
	return os;
}