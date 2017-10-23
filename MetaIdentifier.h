#pragma once

#include "Primary.h"

class CMetaIdentifier : public CPrimary
{
	std::string name;
public:
	CMetaIdentifier();
	virtual ~CMetaIdentifier();
	const std::string& GetName() const;

	std::istream & ReadFrom(std::istream & is) override;
	ISpawnable * spawn() const override;
	static void registerPrefixes();
};

