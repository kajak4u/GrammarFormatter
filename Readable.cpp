#include "Readable.h"

IReadable::~IReadable() {}

std::istream& operator<<(std::istream& is, IReadable& obj)
{
	return obj.ReadFrom(is);
}