#include "Readable.h"

IReadable::~IReadable() {}

_STD istream& operator<<(_STD istream& is, IReadable& obj)
{
	return obj.ReadFrom(is);
}