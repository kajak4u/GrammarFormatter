#pragma once

#include "Spawnable.h"
#include "Readable.h"

class CMultiplier : public ISpawnable, public IReadable
{
	int value;
public:
	CMultiplier();
	virtual ~CMultiplier();

	std::istream & ReadFrom(std::istream & is) override;
	ISpawnable * spawn() const override;
	int GetValue() const;
	static void registerPrefixes();

	// Inherited via IReadable
	virtual void WriteTo(std::ostream & os) const override;
};
std::ostream& operator<<(std::ostream& os, const CMultiplier& mult);