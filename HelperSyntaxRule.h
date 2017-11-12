#pragma once
#include "SyntaxRule.h"
class CGroup;

class CHelperSyntaxRule :
	public CSyntaxRule
{
	const CGroup* originalRule = nullptr;
public:
	virtual ~CHelperSyntaxRule();
	CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CGroup* origin);
	void WriteTo(std::ostream& os) const override;
	CGroup* CreateReplacement() const;
};

