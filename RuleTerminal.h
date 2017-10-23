#pragma once

#include "Readable.h"
#include <string>

class CRuleTerminal : public IReadable
{
	std::string value;
public:
	virtual ~CRuleTerminal();
	std::istream& ReadFrom(std::istream& is) override;
};