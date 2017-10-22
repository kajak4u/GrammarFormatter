#pragma once

#include "RuleElement.h"
#include "Readable.h"
#include <vector>
#include "Enums.h"

class CRuleGroup : public CRuleElement, public IReadable
{
	std::vector<CRuleElement*> content;
	Options options;
public:
	virtual ~CRuleGroup();
	std::istream& ReadFrom(std::istream& is) override;
};