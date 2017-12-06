#pragma once

#include "Primary.h"

class CMetaIdentifierManagerItem;

class CMetaIdentifier : public CPrimary
{
	std::string name;
	CMetaIdentifierManagerItem* item;
public:
	typedef bool(*ComparePointers)(const CMetaIdentifier*, const CMetaIdentifier*);
	CMetaIdentifier(const std::string& name);
	CMetaIdentifier(std::string&& name);
	CMetaIdentifier();
	virtual ~CMetaIdentifier();
	const std::string& GetName() const;

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream& os) const override;
	ISpawnable * spawn(bool copy = false) const override;
	static void registerPrefixes();
	bool operator<(const CMetaIdentifier& other) const;
};

std::ostream& operator<<(std::ostream& os, const CMetaIdentifier& identifier);
