#pragma once

#include "Primary.h"

class CSpecial : public CPrimary
{
	std::string name;
public:
	CSpecial();
	virtual ~CSpecial();

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream & os) const override;
	ISpawnable * spawn() const override;
	static void registerPrefixes();
};

std::ostream& operator<<(std::ostream& os, const CSpecial& special);