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
	CFactor(const CPrimary* primary, int multiplier = 1);
	CFactor(const CFactor& other);
	CFactor(CFactor&&);
	int GetMultiplier() const;
	const CPrimary* GetPrimary() const;
	virtual ~CFactor();

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream & os) const override;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;

};

std::ostream& operator<<(std::ostream& os, const CFactor& factor);