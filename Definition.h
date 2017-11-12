#pragma once

#include "Readable.h"
#include <vector>
#include "GrammarObject.h"
class CTerm;

class CDefinition :	public std::vector<CTerm*>, public IReadable, public CGrammarObject
{
public:
	CDefinition();
	CDefinition(std::istream& is);
	CDefinition(const CDefinition& other);
	CDefinition(CDefinition&& other) = default;
	virtual ~CDefinition();
	std::istream& ReadFrom(std::istream& is) override;
	void WriteTo(std::ostream& os) const override;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;
};
std::ostream& operator<<(std::ostream& os, const CDefinition& def);

