#include "RuleGroup.h"


CRuleGroup::~CRuleGroup()
{
	for (CRuleElement*& elem : content)
		delete elem;
}
