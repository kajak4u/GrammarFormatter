#pragma once

#include "Readable.h"
#include <vector>
#include "GrammarObject.h"

class IDefinition;

class CDefinitionList :	public IReadable, public std::vector<IDefinition*>, public CGrammarObject
{
public:
	CDefinitionList();
	CDefinitionList(const CDefinitionList& other);
	CDefinitionList(CDefinitionList&&) = default;
	virtual ~CDefinitionList();
	CDefinitionList operator=(CDefinitionList&&);
	std::istream& ReadFrom(std::istream& is) override;
	void WriteTo(std::ostream& os) const override;
	void Simplify();

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;

};

std::ostream& operator<<(std::ostream& os, const CDefinitionList& list);
