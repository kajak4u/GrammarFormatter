#pragma once

#include "Readable.h"
#include <vector>
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class IDefinition;

	class CDefinitionList : public IReadable, public _STD vector<IDefinition*>, public CGrammarObject
	{
	public:
		CDefinitionList();
		CDefinitionList(const CDefinitionList& other);
		CDefinitionList(CDefinitionList&&) = default;
		virtual ~CDefinitionList();
		CDefinitionList operator=(CDefinitionList&&);
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream& os) const override;
		void Simplify();

		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;

	};

	_STD ostream& operator<<(_STD ostream& os, const CDefinitionList& list);
}