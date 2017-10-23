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
	void WriteTo(std::ostream& os) const override;
	ISpawnable * spawn() const override;
	static void registerPrefixes();
};

std::ostream& operator<<(std::ostream& os, const CMetaIdentifier& identifier);
