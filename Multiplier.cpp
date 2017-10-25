#include "Multiplier.h"
#include "Recognizer.h"
#include <string>

using namespace std;

CMultiplier::CMultiplier()
{
}


CMultiplier::~CMultiplier()
{
}

std::istream & CMultiplier::ReadFrom(std::istream & is)
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

ISpawnable * CMultiplier::spawn() const
{
	return new CMultiplier();
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

void CMultiplier::WriteTo(std::ostream & os) const
{
	os << value;
}

std::ostream & operator<<(std::ostream & os, const CMultiplier & mult)
{
	mult.WriteTo(os);
	return os;
}