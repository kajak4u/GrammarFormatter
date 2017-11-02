#pragma once

#include "Readable.h"
#include <vector>
#include "GrammarObject.h"
class CDefinition;

class CDefinitionList :	public IReadable, public std::vector<CDefinition*>, public CGrammarObject
{
public:
	CDefinitionList();
	virtual ~CDefinitionList();
	std::istream& ReadFrom(std::istream& is) override;
	void WriteTo(std::ostream& os) const override;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;

};

std::ostream& operator<<(std::ostream& os, const CDefinitionList& list);
