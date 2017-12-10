#pragma once

#include "Primary.h"

namespace GrammarSymbols
{
	class CSpecial : public CPrimary
	{
		_STD string name;
	public:
		CSpecial();
		virtual ~CSpecial();

		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		ISpawnable * spawn(bool copy = false) const override;
		bool Equals(const CPrimary * other) const override;
		static void registerPrefixes();
	};

	_STD ostream& operator<<(_STD ostream& os, const CSpecial& special);
}