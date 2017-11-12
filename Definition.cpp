#include "Definition.h"
#include "Enums.h"
#include "main.h"
#include "Term.h"


CDefinition::CDefinition()
{
}

CDefinition::CDefinition(std::istream & is)
	: CDefinition()
{
	ReadFrom(is);
}

CDefinition::CDefinition(const CDefinition & other)
{
	this->reserve(other.size());
	for (CTerm* term : other)
		this->push_back(new CTerm(*term));
}


CDefinition::~CDefinition()
{
	for (CTerm*& term : *this)
		delete term;
}

std::istream& CDefinition::ReadFrom(std::istream& is)
{
	do
	{
		this->push_back(new CTerm(is));
		skipWhiteChars(is);
		//usun symbol ze strumienia tylko jesli jest to separator
		if (GetSymbol(is, false) == SymbolConcatenate)
			GetSymbol(is, true);
		else
			break;
	} while (true);
	return is;
}

void CDefinition::WriteTo(std::ostream & os) const
{
	bool first = true;
	for (const CTerm* term : *this)
		os << (first ? first = false, "" : ", ") << *term;
}

void CDefinition::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	for (const CTerm* term : *this)
		term->ForEach(condition, action);
}

void CDefinition::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	for (CTerm* term : *this)
		term->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CDefinition & def)
{
	def.WriteTo(os);
	return os;
}
