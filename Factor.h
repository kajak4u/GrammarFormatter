#pragma once

#include "Readable.h"
#include "Primary.h"
#include <functional>
#include "GrammarObject.h"

namespace GrammarSymbols
{

	class CFactor : public IReadable, public CGrammarObject
	{
		int multiplier = 1;
		CPrimary* primary = nullptr;
	public:
		CFactor();
		CFactor(const CPrimary* primary, int multiplier = 1);
		CFactor(const CFactor& other);
		CFactor(CFactor&&);
		int GetMultiplier() const;
		const CPrimary* GetPrimary() const;
		void SetPrimary(const CPrimary* newPrimary);
		virtual ~CFactor();

		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;

		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(const CGrammarObject*)> action) const override;
		void ForEach(_STD function<bool(const CGrammarObject*)> condition, _STD function<void(CGrammarObject*)> action) override;

	};

	_STD ostream& operator<<(_STD ostream& os, const CFactor& factor);
}