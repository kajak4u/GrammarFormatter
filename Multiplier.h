#pragma once

#include "Primary.h"

class CMultiplier : public CPrimary
{
	int value;
public:
	CMultiplier();
	virtual ~CMultiplier();

	std::istream & ReadFrom(std::istream & is) override;
	ISpawnable * spawn() const override;
	int GetValue() const;
	static void registerPrefixes();
};

