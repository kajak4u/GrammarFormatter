#pragma once

#include "DefinedGrammarSymbol.h"
#include "Enums.h"
#include <map>

namespace GrammarSymbols
{
	//represents EBNF 'Special sequence'
	class CSpecial : public CDefinedGrammarSymbol
	{
		//associates special sequences' names with their corresponding formats
		static _STD map<_STD string, FormatEffect> formatMap;

		//current special's name
		_STD string name;
		//current special's format
		FormatEffect format = FormatNone;
	public:
		//empty constructor
		CSpecial();
		//destructor
		virtual ~CSpecial();
		//register its prefixes to CRecognizer
		static void RegisterPrefixes();
		//returns format
		FormatEffect GetFormat() const;

		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from ISpawnable
		ISpawnable * Spawn(bool copy = false) const override;
		//inherited from CPrimary
		bool Equals(const CPrimary * other) const override;
		//inherited from CDefinedGrammarSymbol
		const std::string & GetName() const override;
	};
}