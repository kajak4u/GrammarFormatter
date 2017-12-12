#pragma once

#include "Readable.h"
#include <vector>
#include <set>
#include "Readable.h"
#include "GrammarObject.h"
#include "Terminal.h"
#include "MetaIdentifier.h"

namespace GrammarSymbols
{
	class CSyntaxRule;

	class CSyntax : public _STD vector<CSyntaxRule*>, public IReadable, public CGrammarObject
	{
		int helperRulesCounter = 0;
		CMetaIdentifier startSymbol;
		using Base = _STD vector<CSyntaxRule*>;
	public:
		CSyntax();
		CSyntax(const CSyntax& other);
		CSyntax(CSyntax&&) = default;
		virtual ~CSyntax();

		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		void Simplify();
		void CreateSets();
		bool IsCorrect(_STD string& errors);
//		_STD set<const CMetaIdentifier*, CMetaIdentifier::ComparePointers> GetAllIdentifiers() const;
//		_STD set<const CTerminal*, CTerminal::ComparePointers> GetAllTerminals() const;
		const CMetaIdentifier GetStartSymbol() const;

		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(const CGrammarObject*)> action) const override;
		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(CGrammarObject*)> action) override;
	};

	_STD ostream& operator<<(_STD ostream& os, const CSyntax& syntax);
}