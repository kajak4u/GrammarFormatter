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

	class CShortDefinition : public _STD vector<CPrimary*>, public IDefinition
	{
	public:
		CShortDefinition();
		CShortDefinition(const CShortDefinition& other);
		CShortDefinition(CShortDefinition&&) = default;
		CShortDefinition(CComplexDefinition* previous);
		virtual ~CShortDefinition();

		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream& os) const override;

		ISpawnable * spawn(bool copy = false) const override;
		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(const CGrammarObject*)> action) const override;
		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(CGrammarObject*)> action) override;
	};

	MySet<CTerminal*, CompareObjects<CTerminal>> GetFirstFrom(_STD vector<CPrimary*>::const_iterator begin, _STD vector<CPrimary*>::const_iterator end);
}