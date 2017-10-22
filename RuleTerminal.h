#pragma once

#include "RuleElement.h"
#include "Readable.h"
#include <string>

class CRuleTerminal : public CRuleElement, public IReadable
{
	std::string value;
public:
	virtual ~CRuleTerminal();
	std::istream& ReadFrom(std::istream& is) override;
};