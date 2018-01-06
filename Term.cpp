#include "main.h"
#include "Term.h"

using namespace std;

namespace GrammarSymbols
{
	CTerm::CTerm()
	{
	}

	CTerm::CTerm(_STD istream &is)
	{
		ReadFrom(is);
	}

	CTerm::CTerm(CFactor && factor)
		: factor(_STD move(factor))
	{
	}

	CTerm::CTerm(const CFactor & factor)
		: factor(factor)
	{
	}


	CTerm::CTerm(const CFactor & factor, const CFactor & exception)
		: factor(factor), exception(exception), hasException(true)
	{
	}

	CTerm::~CTerm()
	{
	}

	bool CTerm::HasException() const
	{
		return hasException;
	}

	const CFactor & CTerm::GetFactor() const
	{
		return factor;
	}

	const CFactor & CTerm::GetException() const
	{
		return exception;
	}

	_STD istream & CTerm::ReadFrom(_STD istream & is)
	{
		factor.ReadFrom(is);
		if (GetSymbol(is, false) == SymbolExcept)
		{
			hasException = true;
			GetSymbol(is, true);
			exception.ReadFrom(is);
		}
		return is;
	}

	void CTerm::WriteTo(_STD ostream & os) const
	{
		os << &factor;
		if (hasException)
			os << " - " << &exception;
	}

	void CTerm::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		CGrammarObject::ForEach(condition, action);
		factor.ForEach(condition, action);
		if (hasException)
			exception.ForEach(condition, action);
	}

	void CTerm::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		CGrammarObject::ForEach(condition, action);
		factor.ForEach(condition, action);
		if (hasException)
			exception.ForEach(condition, action);
	}
}