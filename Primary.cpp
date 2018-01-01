#include "Primary.h"

namespace GrammarSymbols
{
	int CPrimary::Compare(const CPrimary * other) const
	{
		//base compare - pointers difference
		return this - other;
	}
	bool CPrimary::operator<(const CPrimary & other) const
	{
		//relation based on Compare()
		return Compare(&other) < 0;
	}

	CPrimary::CPrimary()
	{
	}

	CPrimary::~CPrimary()
	{
	}
}