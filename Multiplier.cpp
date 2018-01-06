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
		//only digits because multiplier should be a natural number; allowing for leading zeros
		while (isdigit(c))
		{
			str += c;
			c = is.get();
		}
		value = stoi(str);
		return is.putback(c);
	}

	ISpawnable * CMultiplier::Spawn(bool copy) const
	{
		return copy ? new CMultiplier(*this) : new CMultiplier();
	}

	int CMultiplier::GetValue() const
	{
		return value;
	}

	void CMultiplier::RegisterPrefixes()
	{
		//register all digits as valid prefixes
		for (char c = '0'; c <= '9'; ++c)
			CRecognizer::RegisterType(new CMultiplier(), string{ c });
	}

	void CMultiplier::WriteTo(_STD ostream & os) const
	{
		os << value;
	}
}