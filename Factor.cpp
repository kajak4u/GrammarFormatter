#include "Factor.h"



CFactor::CFactor()
{
}


CFactor::~CFactor()
{
	if (primary)
		delete primary;
}

std::istream & CFactor::ReadFrom(std::istream & is)
{
	//TODO: Implement CFactor::ReadFrom
	return is;
}
