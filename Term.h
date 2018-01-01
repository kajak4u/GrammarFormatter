#pragma once

#include "Readable.h"
#include "Factor.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	//represents EBNF 'Term', consisting of factor and optional exception
	class CTerm : public IReadable, public CGrammarObject
	{
		//factor
		CFactor factor;
		//whether term has exception clause
		bool hasException = false;
		//exception clause
		CFactor exception;
	public:
		//empty constructor
		CTerm();
		//constructor based on input stream
		CTerm(_STD istream&);
		//move constructor
		CTerm(CFactor&&);
		//copy constructor
		CTerm(const CFactor& factor);
		//constructor
		CTerm(const CFactor& factor, const CFactor& exception);
		//destructor
		virtual ~CTerm();
		//returns whether term has exception
		bool HasException() const;
		//returns factor
		const CFactor& GetFactor() const;
		//returns exception
		const CFactor& GetException() const;

		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from CGrammarObject
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	//output operator stream
	_STD ostream& operator<<(_STD ostream& os, const CTerm& term);
}