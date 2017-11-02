#include "MetaIdentifier.h"
#include "Recognizer.h"
#include "main.h"
#include <string>

using namespace std;

CMetaIdentifier::CMetaIdentifier()
{
}


CMetaIdentifier::~CMetaIdentifier()
{
}

const std::string & CMetaIdentifier::GetName() const
{
	return name;
}

std::istream & CMetaIdentifier::ReadFrom(std::istream & is)
{
	skipWhiteChars(is);
	char c = is.get();
	if (!isalpha(c))
		throw invalid_argument(string()+"Meta identifier should start with letter, '"+c+"' found instead.");

	do
	{
		name += c;
		c = is.get();
	} while (isalnum(c) || c==' ' /*|| c=='-'*/);
	while (name.back() == ' ')
		name.pop_back();

	return is.putback(c);
}

void CMetaIdentifier::WriteTo(std::ostream & os) const
{
	os << name;
}

ISpawnable * CMetaIdentifier::spawn() const
{
	return new CMetaIdentifier();
}

void CMetaIdentifier::registerPrefixes()
{
	for (char c = 'a'; c <= 'z'; ++c)
	{
		CRecognizer::registerType(new CMetaIdentifier(), string{c});
		CRecognizer::registerType(new CMetaIdentifier(), string{ (char)toupper(c) });
	}
}

bool CMetaIdentifier::operator<(const CMetaIdentifier & other) const
{
	return name < other.name;
}

std::ostream & operator<<(std::ostream & os, const CMetaIdentifier & identifier)
{
	identifier.WriteTo(os);
	return os;
}
