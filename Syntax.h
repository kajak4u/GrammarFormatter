#pragma once

#include "Readable.h"
#include <vector>
#include <set>
#include "Readable.h"
#include "GrammarObject.h"
#include "MetaIdentifier.h"

namespace GrammarSymbols
{
	class CSyntaxRule;
	//represents EBNF syntax
	class CSyntax : public _STD vector<CSyntaxRule*>, public IReadable, public CGrammarObject
	{
		//counter for helper rules
		int helperRulesCounter = 0;
		//syntax's starting symbol
		CMetaIdentifier startSymbol;
	public:
		//empty constructor
		CSyntax();
		//copy constructor
		CSyntax(const CSyntax& other);
		//move constructor - default
		CSyntax(CSyntax&&) = default;
		//destructor
		virtual ~CSyntax();
		//simplifies all rules, converting CComplexDefinitionobjects into CShortDefinition ones
		void Simplify();
		//builds FIRST and FOLLOW sets for all symbols in grammar
		void CreateSets();
		//checks whether the syntax is correct
		bool IsCorrect(_STD string& errors);
		//returns syntax's start symbol
		const CMetaIdentifier* GetStartSymbol() const;


		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CSyntax& syntax);
}