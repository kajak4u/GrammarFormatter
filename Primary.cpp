#include "Primary.h"

namespace GrammarSymbols
{
	int CPrimary::Compare(const CPrimary * other) const
	{
		return this - other;
	}
	bool CPrimary::operator<(const CPrimary & other) const
	{
		return Compare(&other) < 0;
	}
	CPrimary::CPrimary()
	{
	}


	CPrimary::~CPrimary()
	{
	}
}