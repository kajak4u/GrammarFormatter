#pragma once
#include "Primary.h"
#include "MySet.h"
#include "main.h"

namespace GrammarSymbols
{
	class CTerminal;
	class CDefinition;
	class CShortDefinition;
	struct CDefinedSymbolManagerItem;

	//defined grammar symbol - a grammar symbol that is derivated from some rules or empty symbol
	class CDefinedGrammarSymbol : public CPrimary
	{
		//corresponding additional global data
		CDefinedSymbolManagerItem* item;
	protected:
		//returns corresponding data
		CDefinedSymbolManagerItem* GetItem() const;
		//registers symbol to get access to additional data
		void Register();
	public:
		//constructor
		CDefinedGrammarSymbol();
		//destructor
		virtual ~CDefinedGrammarSymbol();

		//access FIRST set corresponding to symbol
		MySet<CTerminal*, CompareObjects<CTerminal>>& First() const;
		//try to extend FIRST set based on given definition; returns whether anything was added
		bool TryAddFirstFrom(const CShortDefinition * def) const;
		//access FOLLOW set corresponding to symbol
		MySet<CTerminal*, CompareObjects<CTerminal>>& Follow() const;
		//get all rules that may be used to receive symbol
		const MySet<const CDefinition*>& GetDefinitions() const;
		//perform an action for each symbol in memory
		static void ForEach(_STD function<void(const CDefinedGrammarSymbol*)> func);

		//derrived from CPrimary
		virtual int Compare(const CPrimary* other) const override;
		//get symbol's name
		virtual const std::string & GetName() const = 0;
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CDefinedGrammarSymbol& identifier);
}