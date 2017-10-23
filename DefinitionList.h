#pragma once

#include "Readable.h"
#include "Definition.h"
#include <vector>

class CDefinitionList :	public IReadable, public std::vector<CDefinition>
{
public:
	CDefinitionList();
	virtual ~CDefinitionList();
	std::istream& ReadFrom(std::istream& is) override;
};

