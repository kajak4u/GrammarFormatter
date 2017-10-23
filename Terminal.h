#pragma once

#include <string>
#include "Primary.h"

class CTerminal : public CPrimary
{
	std::string value;
public:
	virtual ~CTerminal();
	std::istream& ReadFrom(std::istream& is) override;
	ISpawnable * spawn() const override;
	static void registerPrefixes();
};