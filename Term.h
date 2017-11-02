#pragma once

#include "Readable.h"
#include "Factor.h"
#include <functional>
#include "GrammarObject.h"

class CTerm : public IReadable, public CGrammarObject
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

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
};
std::ostream& operator<<(std::ostream& os, const CTerm& term);