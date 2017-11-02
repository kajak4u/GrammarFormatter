#pragma once

#include "Readable.h"
#include "Spawnable.h"
#include "GrammarObject.h"

class CPrimary : public IReadable, public ISpawnable, public CGrammarObject
{
public:
	CPrimary();
	virtual ~CPrimary();
};

