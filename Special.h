#pragma once

#include "DefinedGrammarSymbol.h"
#include "Enums.h"
#include <map>

namespace GrammarSymbols
{
	class CSpecial : public CDefinedGrammarSymbol
	{
		_STD string name;
		static _STD map<_STD string, FormatEffect> formatMap;
		FormatEffect format = FormatNone;
	public:
		CSpecial();
		virtual ~CSpecial();

		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		ISpawnable * spawn(bool copy = false) const override;
		bool Equals(const CPrimary * other) const override;
		static void registerPrefixes();
		FormatEffect getFormat() const;

		const std::string & GetName() const override;
	};

	_STD ostream& operator<<(_STD ostream& os, const CSpecial& special);
}