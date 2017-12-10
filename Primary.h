#pragma once

#include "Readable.h"
#include "Spawnable.h"
#include "GrammarObject.h"

class CPrimary : public IReadable, public ISpawnable, public CGrammarObject
{
public:
	virtual bool Equals(const CPrimary* other) const = 0;
	CPrimary();
	virtual ~CPrimary();
};

