#pragma once

#include "Readable.h"
#include "Primary.h"
#include <functional>
#include "GrammarObject.h"

class CFactor :	public IReadable, public CGrammarObject
{
	int multiplier = 1;
	CPrimary* primary = nullptr;
public:
	CFactor();
	CFactor(const CFactor& other);
	CFactor(CFactor&&) = default;
	virtual ~CFactor();

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream & os) const override;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;

};

std::ostream& operator<<(std::ostream& os, const CFactor& factor);