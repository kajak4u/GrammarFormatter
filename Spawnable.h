#pragma once
#include <iostream>

class ISpawnable
{
public:
	virtual ISpawnable* spawn(bool copy=false) const = 0;
};