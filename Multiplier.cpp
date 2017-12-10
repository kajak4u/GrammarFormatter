#include "Multiplier.h"
#include "Recognizer.h"
#include <string>

using namespace std;

namespace GrammarSymbols
{
	CMultiplier::CMultiplier()
	{
	}


	CMultiplier::~CMultiplier()
	{
	}

	_STD istream & CMultiplier::ReadFrom(_STD istream & is)
	{
		string str;
		char c = is.get();
		while (isdigit(c))
		{
			str += c;
			c = is.get();
		}
		value = stoi(str);
		return is.putback(c);
	}

	ISpawnable * CMultiplier::spawn(bool copy) const
	{
		return copy ? new CMultiplier(*this) : new CMultiplier();
	}

	int CMultiplier::GetValue() const
	{
		return value;
	}

	void CMultiplier::registerPrefixes()
	{
		for (char c = '0'; c <= '9'; ++c)
			CRecognizer::registerType(new CMultiplier(), string{ c });
	}

	void CMultiplier::WriteTo(_STD ostream & os) const
	{
		os << value;
	}

	_STD ostream & operator<<(_STD ostream & os, const CMultiplier & mult)
	{
		mult.WriteTo(os);
		return os;
	}
}