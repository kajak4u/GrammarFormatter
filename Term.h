#pragma once

#include "Readable.h"
#include "Factor.h"

class CTerm : public IReadable
{
	CFactor factor;
	CFactor exception;
public:
	CTerm();
	CTerm(std::istream&);
	virtual ~CTerm();

	std::istream & ReadFrom(std::istream & is) override;
};

