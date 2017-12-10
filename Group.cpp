#include "main.h"
#include "Group.h"
#include "Recognizer.h"

using namespace std;


Option CGroup::GetType() const
{
	return options;
}

CGroup::CGroup()
{
}

CGroup::CGroup(const CDefinitionList & list, Option type)
	: definitionList(list), options(type)
{
}

CGroup::CGroup(Option type)
	: options(type)
{
}
//
//CGroup & CGroup::operator=(const CGroup & other)
//{
//	definitionList = other.definitionList;
//	options = other.options;
//}

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
		options = OptionOptional;
		break;
	case '{':
		endch = '}';
		options = OptionRepetition;
		break;
	case '(':
		endch2 = ')';
		endch = c2 = is.get();
		switch (c2)
		{
		case '/':
			options = OptionOptional;
			break;
		case ':':
			options = OptionRepetition;
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

const CDefinitionList & CGroup::getDefinitionList() const
{
	return definitionList;
}

ISpawnable* CGroup::spawn(bool copy) const
{
	return copy ? new CGroup(*this) : new CGroup();
}

void CGroup::registerPrefixes()
{
	CRecognizer::registerType(new CGroup(), "[");
	CRecognizer::registerType(new CGroup(), "(/");

	CRecognizer::registerType(new CGroup(), "{");
	CRecognizer::registerType(new CGroup(), "(:");

	CRecognizer::registerType(new CGroup(), "(");
}

void CGroup::WriteTo(std::ostream & os) const
{
	string brackets = options == OptionOptional ? "[]" : options == OptionRepetition ? "{}" : "()";
	os << brackets[0] << definitionList << brackets[1];
}


void CGroup::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	definitionList.ForEach(condition, action);
}

void CGroup::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	definitionList.ForEach(condition, action);
}

bool CGroup::Equals(const CPrimary * other) const
{
	const CGroup* mi = dynamic_cast<const CGroup*>(other);
	return mi != nullptr && mi->options == options && mi->definitionList == definitionList;
}

std::ostream & operator<<(std::ostream & os, const CGroup & group)
{
	group.WriteTo(os);
	return os;
}
