#pragma once

#include "Readable.h"
#include <vector>

class CTerm;

class CDefinition :	public std::vector<CTerm*>, public IReadable
{
public:
	CDefinition();
	CDefinition(std::istream& is);
	virtual ~CDefinition();
	std::istream& ReadFrom(std::istream& is) override;
	void WriteTo(std::ostream& os) const override;
};
std::ostream& operator<<(std::ostream& os, const CDefinition& def);

