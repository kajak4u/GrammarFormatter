#pragma once

#include "Primary.h"
#include "MySet.h"

struct CMetaIdentifierManagerItem;
class CTerminal;
class CShortDefinition;
class IDefinition;


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
	bool Equals(const CPrimary * other) const override;
	static void registerPrefixes();
	bool operator<(const CMetaIdentifier& other) const;
	void MarkAsDefinedBy(const IDefinition*) const;
	void MarkAsUsed() const;

	MySet<CTerminal*>& First() const;
	bool TryAddFirstFrom(const CShortDefinition* def) const;
	MySet<CTerminal*>& Follow() const;

	const MySet<const IDefinition*>& GetDefinitions() const;
	static bool GetWarnings(MySet<std::string>& undefined, MySet<std::string>& unused);
};

std::ostream& operator<<(std::ostream& os, const CMetaIdentifier& identifier);
