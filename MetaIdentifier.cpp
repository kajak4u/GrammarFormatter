#include "MetaIdentifier.h"
#include "Recognizer.h"

using namespace std;

CMetaIdentifier::CMetaIdentifier()
{
}


CMetaIdentifier::~CMetaIdentifier()
{
}

std::istream & CMetaIdentifier::ReadFrom(std::istream & is)
{
	char c = is.get();
	if (!isalpha(c))
		throw invalid_argument(string()+"Meta identifier should start with letter, '"+c+"' found instead.");

	do
	{
		name += c;
		c = is.get();
	} while (isalnum(c));

	return is.putback(c);
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
