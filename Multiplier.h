#pragma once

#include "Spawnable.h"
#include "Readable.h"
#include "GrammarObject.h"

class CMultiplier : public ISpawnable, public IReadable, public CGrammarObject
{
	int value;
public:
	CMultiplier();
	virtual ~CMultiplier();

	std::istream & ReadFrom(std::istream & is) override;
	ISpawnable * spawn(bool copy = false) const override;
	int GetValue() const;
	static void registerPrefixes();

	void WriteTo(std::ostream & os) const override;
};
std::ostream& operator<<(std::ostream& os, const CMultiplier& mult);