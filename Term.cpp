#include "Term.h"
#include "main.h"

using namespace std;

CTerm::CTerm()
{
}

CTerm::CTerm(std::istream &is)
{
	ReadFrom(is);
}

CTerm::CTerm(CFactor && factor)
	: factor(std::move(factor))
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

std::istream & CTerm::ReadFrom(std::istream & is)
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

void CTerm::WriteTo(std::ostream & os) const
{
	os << factor;
	if (hasException)
		os << " - " << exception;
}

void CTerm::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	factor.ForEach(condition, action);
	if (hasException)
		exception.ForEach(condition, action);
}

void CTerm::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	factor.ForEach(condition, action);
	if (hasException)
		exception.ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CTerm & term)
{
	term.WriteTo(os);
	return os;
}
