#pragma once

#include <vector>
#include "Definition.h"

namespace GrammarSymbols
{

	class CTerm;

	//complex definition - definition directly from EBNF standard
	class CComplexDefinition : public _STD vector<CTerm*>, public CDefinition
	{
	public:
		//constructor
		CComplexDefinition();
		//constructor using input stream
		CComplexDefinition(_STD istream& is);
		//copy constructor
		CComplexDefinition(const CComplexDefinition& other);
		//move constructor - default
		CComplexDefinition(CComplexDefinition&& other) = default;
		//destructor
		virtual ~CComplexDefinition();

		//inherited from IReadale
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream& os) const override;
		//inherited from ISpawnable
		ISpawnable * Spawn(bool copy = false) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
	};
}