#pragma once

#include "Readable.h"
#include "MetaIdentifier.h"
#include "DefinitionList.h"
#include "GrammarObject.h"

using namespace std;

namespace GrammarSymbols
{
	//represents EBNF single syntax rule
	class CSyntaxRule : public IReadable, public CGrammarObject
	{
		//rule's identifier
		CMetaIdentifier identifier;
		//definition list for rule
		CDefinitionList definitionList;
	public:
		//empty constructor
		CSyntaxRule();
		//copy constructor
		CSyntaxRule(const CMetaIdentifier& identifier);
		//constructor based on input stream
		CSyntaxRule(_STD istream& is);
		//destructor
		virtual ~CSyntaxRule();
		//returns identifier
		const CMetaIdentifier& GetIdentifier() const;
		//returns definition list
		const CDefinitionList& GetDefinitionList() const;
		//add definition to list
		void AddDefinition(CDefinition* definition);
		//add copy of definition to list
		void AddCopyDefinition(const CDefinition* definition);
		//simplifies the rule, converting all CComplexDefinition objects into CShortDefinition ones
		void Simplify();

		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		virtual void WriteTo(_STD ostream& os) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CSyntaxRule& rule);
}