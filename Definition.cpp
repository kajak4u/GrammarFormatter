#include "Definition.h"


CDefinition::CDefinition()
{
}

CDefinition::CDefinition(std::istream & is)
	: CDefinition()
{
	ReadFrom(is);
}


CDefinition::~CDefinition()
{
}

std::istream& CDefinition::ReadFrom(std::istream& is)
{
	//TODO: Implement CDefinition::ReadFrom
	return is;
}