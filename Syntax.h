#pragma once

#include "Readable.h"
#include <vector>
#include "Readable.h"

class CSyntaxRule;

class CSyntax :	public std::vector<CSyntaxRule*>, public IReadable
{
public:
	CSyntax();
	virtual ~CSyntax();

	std::istream & ReadFrom(std::istream & is) override;
};

