#pragma once

#include "Primary.h"
#include "MySet.h"

struct CMetaIdentifierManagerItem;
class CTerminal;
class CShortDefinition;


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
	void MarkAsDefined() const;
	void MarkAsUsed() const;

	MySet<CTerminal*>& First() const;
	bool TryAddFirstFrom(const CShortDefinition* def) const;
	MySet<CTerminal*>& Follow() const;

	static bool GetWarnings(MySet<std::string>& undefined, MySet<std::string>& unused);
};

std::ostream& operator<<(std::ostream& os, const CMetaIdentifier& identifier);
