#pragma once

#include "Readable.h"
#include "Factor.h"
#include <functional>
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class CTerm : public IReadable, public CGrammarObject
	{
		CFactor factor;
		bool hasException = false;
		CFactor exception;
	public:
		CTerm();
		CTerm(_STD istream&);
		CTerm(CFactor&&);
		CTerm(const CFactor& factor);
		CTerm(const CFactor& factor, const CFactor& exception);
		virtual ~CTerm();

		bool HasException() const;
		const CFactor& GetFactor() const;
		const CFactor& GetException() const;

		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;

		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	_STD ostream& operator<<(_STD ostream& os, const CTerm& term);
}