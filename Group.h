#pragma once

#include <vector>
#include "Enums.h"
#include "DefinitionList.h"
#include "Primary.h"

namespace GrammarSymbols
{
	//represents EBNF 'Grouped sequence', 'Optional sequence' and 'Repeated sequence'
	class CGroup : public CPrimary
	{
		//definition list in group's expansion (not a single definition because it can be separated by '|' sign)
		CDefinitionList definitionList;
		//group type - none () / repeat {} / optional []
		GroupType type = GroupDefault;
	public:
		//returns group type
		GroupType GetType() const;
		//empty constructor
		CGroup();
		//copy constructor - default
		CGroup(const CGroup& other) = default;
		//move constructor - default
		CGroup(CGroup&& other) = default;
		//constructor
		CGroup(const CDefinitionList& list, GroupType type);
		//constructor with empty definition list
		CGroup(GroupType type);
		//move assign operator - default
		CGroup& operator=(CGroup&& other) = default;
		//destructor
		virtual ~CGroup();
		//returns its definition list
		const CDefinitionList& getDefinitionList() const;
		//register its prefixes to CRecognizer
		static void registerPrefixes();

		//inherited from ISpawnable
		ISpawnable* spawn(bool copy = false) const override;
		//inherited from IReadable
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from CPrimary
		bool Equals(const CPrimary * other) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CGroup& group);
}