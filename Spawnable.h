#pragma once

//interface for objects' spawning and copying
class ISpawnable
{
public:
	virtual ISpawnable* spawn(bool copy=false) const = 0;
	virtual ~ISpawnable() {}
};