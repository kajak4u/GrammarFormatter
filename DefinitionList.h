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

		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(const CGrammarObject*)> action) const override;
		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(CGrammarObject*)> action) override;

	};

	_STD ostream& operator<<(_STD ostream& os, const CDefinitionList& list);
}