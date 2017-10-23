#pragma once

#include "Readable.h"
#include "Spawnable.h"

class CPrimary : public IReadable, public ISpawnable
{
public:
	CPrimary();
	virtual ~CPrimary();
};

