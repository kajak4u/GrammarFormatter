#pragma once

#include <string>
#include "Primary.h"
#include "GrammarObject.h"

class CTerminalManagerItem;

class CTerminal : public CPrimary
{
	std::string value;
	CTerminalManagerItem* item;
public:
	CTerminal();
	CTerminal(const std::string& value);
	CTerminal(std::string&& value);
	typedef bool(*ComparePointers)(const CTerminal*, const CTerminal*);
	const std::string& GetValue() const;
	virtual ~CTerminal();
	std::istream& ReadFrom(std::istream& is) override;
	ISpawnable * spawn(bool copy = false) const override;
	static void registerPrefixes();
	CTerminal& operator=(const CTerminal& other);

	bool operator<(const CTerminal& other) const;

	void WriteTo(std::ostream & os) const override;
	bool Equals(const CPrimary * other) const override;
	static CTerminal* CreateUnique();
};
std::ostream& operator<<(std::ostream& os, const CTerminal& terminal);