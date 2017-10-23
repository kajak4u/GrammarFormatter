#pragma once

#include "Readable.h"
#include "Factor.h"

class CTerm : public IReadable
{
	CFactor factor;
	bool hasException = false;
	CFactor exception;
public:
	CTerm();
	CTerm(std::istream&);
	virtual ~CTerm();

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream & os) const override;
};
std::ostream& operator<<(std::ostream& os, const CTerm& term);