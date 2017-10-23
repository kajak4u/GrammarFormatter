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
	void WriteTo(std::ostream & os) const override;
};

std::ostream& operator<<(std::ostream& os, const CFactor& factor);