#pragma once

#include <vector>
#include "Definition.h"
#include "MySet.h"

namespace GrammarSymbols
{

	class CTerm;

	//complex definition - definition directly from EBNF standard
	class CComplexDefinition : public _STD vector<CTerm*>, public IDefinition
	{
	public:
		CComplexDefinition();
		CComplexDefinition(_STD istream& is);
		CComplexDefinition(const CComplexDefinition& other);
		CComplexDefinition(CComplexDefinition&& other) = default;
		virtual ~CComplexDefinition();
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream& os) const override;

		ISpawnable * spawn(bool copy = false) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
	};
	_STD ostream& operator<<(_STD ostream& os, const CComplexDefinition& def);
}