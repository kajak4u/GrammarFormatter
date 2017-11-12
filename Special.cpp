#include "Special.h"
#include "Recognizer.h"
#include "main.h"
#include <string>

using namespace std;



CSpecial::CSpecial()
{
}


CSpecial::~CSpecial()
{
}

std::istream & CSpecial::ReadFrom(std::istream & is)
{
	skipWhiteChars(is);
	char c = is.get();
	if (c!='?')
		throw invalid_argument(string() + "Special sequence should start with question mark, '" + c + "' found instead.");
	skipWhiteChars(is);

	while(!is.eof() && (c=is.get())!='?')
		name += c;
	if(c!='?')
		throw invalid_argument(string() + "Special sequence should end with question mark, '" + c + "' found instead.");

	while (name.back() == ' ')
		name.pop_back();
	return is;
}

void CSpecial::WriteTo(std::ostream & os) const
{
	os << "?" << name << "?";
}

ISpawnable * CSpecial::spawn(bool copy) const
{
	return copy ? new CSpecial(*this) : new CSpecial();
}

void CSpecial::registerPrefixes()
{
	CRecognizer::registerType(new CSpecial(), "?");
}

std::ostream & operator<<(std::ostream & os, const CSpecial & special)
{
	special.WriteTo(os);
	return os;
}
