#pragma once

#include "Readable.h"
#include <vector>
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class CDefinition;
	//list of grammar definitions in EBNF
	class CDefinitionList : public IReadable, public _STD vector<CDefinition*>, public CGrammarObject
	{
	public:
		//constructor
		CDefinitionList();
		//copy constructor
		CDefinitionList(const CDefinitionList& other);
		//move constructor - default
		CDefinitionList(CDefinitionList&&) = default;
		//destructor
		virtual ~CDefinitionList();
		//move assignment
		CDefinitionList operator=(CDefinitionList&&);
		//convert CComplexDefinition items into CShortDefinition ones
		void Simplify();

		//inherited from IReadable
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream& os) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;

	};
}