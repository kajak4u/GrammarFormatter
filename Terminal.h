#pragma once

#include "Readable.h"
#include <string>

class CTerminal : public IReadable
{
	std::string value;
public:
	virtual ~CTerminal();
	std::istream& ReadFrom(std::istream& is) override;
};