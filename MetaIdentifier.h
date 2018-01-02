#pragma once

#include "MySet.h"
#include "DefinedGrammarSymbol.h"

namespace GrammarSymbols
{
	class CTerminal;
	class CShortDefinition;
	class CDefinition;

	//represents EBNF 'meta identifier'
	class CMetaIdentifier : public CDefinedGrammarSymbol
	{
		//identifier's name
		_STD string name;
	public:
		//copy constructor
		CMetaIdentifier(const _STD string& name);
		//move constructor
		CMetaIdentifier(_STD string&& name);
		//empty constructor
		CMetaIdentifier();
		//destructor
		virtual ~CMetaIdentifier();
		//register its prefixes to CRecognizer
		static void RegisterPrefixes();
		bool operator<(const CMetaIdentifier& other) const;
		//marks identifier's name as defined by given definition
		void MarkAsDefinedBy(const CDefinition*) const;
		//mark identifier's name as used in another definition
		void MarkAsUsed() const;
		//returns all undefined or unused identifiers
		static bool GetWarnings(MySet<_STD string>& undefined, MySet<_STD string>& unused);

		//inherited from CDefinedGrammarSymbol
		const _STD string& GetName() const override;
		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream& os) const override;
		//inherited from ISpawnable
		ISpawnable * Spawn(bool copy = false) const override;
		//inherited from CPrimary
		bool Equals(const CPrimary * other) const override;
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CMetaIdentifier& identifier);
}