#pragma once
#include "Primary.h"
#include "MySet.h"
#include "main.h"

namespace GrammarSymbols
{
	class CTerminal;
	class IDefinition;
	class CShortDefinition;
	struct CDefinedSymbolManagerItem;

	class CDefinedGrammarSymbol :
		public CPrimary
	{
		CDefinedSymbolManagerItem* item;
	protected:
		CDefinedSymbolManagerItem* GetItem() const;
		void Register();
	public:
		CDefinedGrammarSymbol() {}
		virtual ~CDefinedGrammarSymbol() {}

		MySet<CTerminal*, CompareObjects<CTerminal>>& First() const;
		bool TryAddFirstFrom(const CShortDefinition * def) const;
		MySet<CTerminal*, CompareObjects<CTerminal>>& Follow() const;
		const MySet<const IDefinition*>& GetDefinitions() const;
		virtual int Compare(const CPrimary* other) const override;

		virtual const std::string & GetName() const = 0;
		static void ForEach(_STD function<void(const CDefinedGrammarSymbol*)> func);
	};

	_STD ostream& operator<<(_STD ostream& os, const CDefinedGrammarSymbol& identifier);
}