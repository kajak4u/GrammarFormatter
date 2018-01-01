#pragma once

#include "Readable.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class CPrimary;

	//factor class from EBNF standard - holds multiplier and primary
	class CFactor : public IReadable, public CGrammarObject
	{
		//primary's multiplier
		int multiplier = 1;
		//primary symbol
		CPrimary* primary = nullptr;
	public:
		//empty constructor
		CFactor();
		//constructor
		CFactor(const CPrimary* primary, int multiplier = 1);
		//copy constructor
		CFactor(const CFactor& other);
		//move constructor
		CFactor(CFactor&&);
		//destructor
		virtual ~CFactor();
		//returns multiplier
		int GetMultiplier() const;
		//returns primary
		const CPrimary* GetPrimary() const;
		//sets primary
		void SetPrimary(const CPrimary* newPrimary);

		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;

	};

	_STD ostream& operator<<(_STD ostream& os, const CFactor& factor);
}