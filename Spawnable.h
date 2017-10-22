#pragma once
#include <iostream>

class ISpawnable
{
public:
	virtual ISpawnable* spawn() const = 0;
};