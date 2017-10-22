#pragma once

#include "Readable.h"
#include <vector>
#include "Term.h"

class CDefinition :	public std::vector<CTerm>, public IReadable
{
public:
	CDefinition();
	virtual ~CDefinition();
	std::istream& ReadFrom(std::istream& is) override;
};

