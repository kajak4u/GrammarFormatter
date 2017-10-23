#pragma once

#include "Readable.h"
#include "Factor.h"

class CTerm :
	public IReadable
{
	CFactor factor;
	CFactor exception;
public:
	CTerm();
	virtual ~CTerm();
};

