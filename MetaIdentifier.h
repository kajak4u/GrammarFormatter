#pragma once

#include "Primary.h"
#include "MySet.h"
#include "main.h"
#include "DefinedGrammarSymbol.h"

namespace GrammarSymbols
{

	struct CMetaIdentifierManagerItem;
	class CTerminal;
	class CShortDefinition;
	class IDefinition;


	class CMetaIdentifier : public CDefinedGrammarSymbol
	{
		_STD string name;
	public:
		CMetaIdentifier(const _STD string& name);
		CMetaIdentifier(_STD string&& name);
		CMetaIdentifier();
		virtual ~CMetaIdentifier();
		const _STD string& GetName() const override;

		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream& os) const override;
		ISpawnable * spawn(bool copy = false) const override;
		bool Equals(const CPrimary * other) const override;
		static void registerPrefixes();
		bool operator<(const CMetaIdentifier& other) const;
		void MarkAsDefinedBy(const IDefinition*) const;
		void MarkAsUsed() const;

		static bool GetWarnings(MySet<_STD string>& undefined, MySet<_STD string>& unused);
	};

	_STD ostream& operator<<(_STD ostream& os, const CMetaIdentifier& identifier);
}