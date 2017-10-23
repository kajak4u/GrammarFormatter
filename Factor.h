#pragma once

#include "Readable.h"
#include "Primary.h"

class CFactor :	public IReadable
{
	int multiplier = 1;
	CPrimary* primary = nullptr;
public:
	CFactor();
	virtual ~CFactor();

	std::istream & ReadFrom(std::istream & is) override;
};

