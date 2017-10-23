#pragma once

#include "Primary.h"

class CMetaIdentifier : public CPrimary
{
	std::string name;
public:
	CMetaIdentifier();
	virtual ~CMetaIdentifier();

	std::istream & ReadFrom(std::istream & is) override;
	ISpawnable * spawn() const override;
	static void registerPrefixes();
};

