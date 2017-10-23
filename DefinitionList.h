#pragma once

#include "Readable.h"
#include <vector>

class CDefinition;

class CDefinitionList :	public IReadable, public std::vector<CDefinition*>
{
public:
	CDefinitionList();
	virtual ~CDefinitionList();
	std::istream& ReadFrom(std::istream& is) override;
};

