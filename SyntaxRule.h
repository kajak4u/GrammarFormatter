#pragma once

#include "Readable.h"
#include "MetaIdentifier.h"
#include "DefinitionList.h"
#include <functional>
#include "GrammarObject.h"

using namespace std;

namespace GrammarSymbols
{
	class CSyntaxRule : public IReadable, public CGrammarObject
	{
		CMetaIdentifier identifier;
		CDefinitionList definitionList;
	public:
		CSyntaxRule();
		CSyntaxRule(const CMetaIdentifier& identifier);
		CSyntaxRule(_STD istream& is);
		virtual ~CSyntaxRule();
		const CMetaIdentifier& GetIdentifier() const;
		const CDefinitionList& GetDefinitionList() const;

		void AddDefinition(IDefinition* definition);
		void AddCopyDefinition(const IDefinition* definition);

		void Simplify();

		_STD istream & ReadFrom(_STD istream & is) override;
		virtual void WriteTo(_STD ostream& os) const override;


		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(const CGrammarObject*)> action) const override;
		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(CGrammarObject*)> action) override;

	};
	_STD ostream& operator<<(_STD ostream& os, const CSyntaxRule& rule);
}