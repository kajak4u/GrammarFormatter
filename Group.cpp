#include "main.h"
#include "Group.h"
#include "Recognizer.h"

using namespace std;


CGroup::~CGroup()
{
}

std::istream& CGroup::ReadFrom(std::istream& is)
{
	char c = is.get(), c2;
	char endch, endch2='\0';
	switch (c)
	{
	case '[':
		endch = ']';
		options |= OptionOptional;
		break;
	case '{':
		endch = '}';
		options |= OptionRepetition;
		break;
	case '(':
		endch2 = ')';
		endch = c2 = is.get();
		switch (c2)
		{
		case '/':
			options |= OptionOptional;
			break;
		case ':':
			options |= OptionRepetition;
			break;
		default:
			endch = endch2;
			endch2 = '\0';
			is.putback(c2);
		}
		break;
	default:
		throw invalid_argument(string()+"Group cannot begin with '"+c+"'");
		break;
	}
	is << definitionList;
	skipWhiteChars(is);
	c = is.get();
	if (c != endch)
		throw invalid_argument(string() + "Expected end-of-group symbol '" + endch + "', got '" + c + "'");
	if (endch2 != '\0' && (c = is.get()) != endch2)
		throw invalid_argument(string() + "Expected end-of-group symbol '" + endch + endch2 + "', got '" + endch + c + "'");

	return is;
}

ISpawnable* CGroup::spawn() const
{
	return new CGroup();
}

void CGroup::registerPrefixes()
{
	CRecognizer::registerType(new CGroup(), "[");
	CRecognizer::registerType(new CGroup(), "(/");

	CRecognizer::registerType(new CGroup(), "{");
	CRecognizer::registerType(new CGroup(), "(:");

	CRecognizer::registerType(new CGroup(), "(");
}