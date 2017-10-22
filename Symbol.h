#pragma once

#include "Readable.h"
#include <vector>
#include "RuleGroup.h"

class CSymbol : public IReadable
{
	std::string name;
	std::vector<CRuleGroup> rules; // name = rule[0] | rule[1] | ... ;
public:
	virtual ~CSymbol();
	std::istream& ReadFrom(std::istream& is) override;
};