#pragma once

#include <iostream>

class IReadable
{
public:
	virtual std::istream& ReadFrom(std::istream& is) = 0;
	virtual ~IReadable();
};
std::istream& operator<<(std::istream& is, IReadable& obj);