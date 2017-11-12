#pragma once

#include <string>
#include "Primary.h"
#include "GrammarObject.h"

class CTerminal : public CPrimary
{
	std::string value;
public:
	typedef bool(*ComparePointers)(const CTerminal*, const CTerminal*);
	const std::string& GetValue() const;
	virtual ~CTerminal();
	std::istream& ReadFrom(std::istream& is) override;
	ISpawnable * spawn(bool copy = false) const override;
	static void registerPrefixes();
	CTerminal& operator=(const CTerminal& other);

	void WriteTo(std::ostream & os) const override;
};
std::ostream& operator<<(std::ostream& os, const CTerminal& terminal);