#pragma once

#include <vector>
#include "Definition.h"
#include "MySet.h"
#include "main.h"

namespace GrammarSymbols
{
	class CPrimary;
	class CComplexDefinition;
	class CTerminal;

	//represents simplified definition, consisting of terminals, identifiers and special symbols
	class CShortDefinition : public _STD vector<CPrimary*>, public CDefinition
	{
	public:
		//constructor for empty definition
		CShortDefinition();
		//copy constructor
		CShortDefinition(const CShortDefinition& other);
		//move constructor - default
		CShortDefinition(CShortDefinition&&) = default;
		//constructor from previous complex definition
		CShortDefinition(CComplexDefinition* previous);
		//constructor from initializer list
		CShortDefinition(_STD initializer_list<CPrimary*> list);
		//destructor
		virtual ~CShortDefinition();

		//inherited from IReadable
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream& os) const override;
		//inherited from ISpawnable
		ISpawnable * spawn(bool copy = false) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	//returns FIRST set for corresponding definition fragment
	MySet<CTerminal*, CompareObjects<CTerminal>> GetFirstFrom(_STD vector<CPrimary*>::const_iterator begin, _STD vector<CPrimary*>::const_iterator end);
}