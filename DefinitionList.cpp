#include "DefinitionList.h"
#include "main.h"
#include "Definition.h"


CDefinitionList::CDefinitionList()
{
}

CDefinitionList::CDefinitionList(const CDefinitionList & other)
{
	this->reserve(other.size());
	for (CDefinition* definition : other)
		this->push_back(new CDefinition(*definition));
}


CDefinitionList::~CDefinitionList()
{
	for (CDefinition*& def : *this)
		delete def;
}

CDefinitionList CDefinitionList::operator=(CDefinitionList && other)
{
	*((vector<CDefinition*>*)this) = std::move((vector<CDefinition*>)other);
	other.clear();
	return *this;
}

std::istream& CDefinitionList::ReadFrom(std::istream& is)
{
	do
	{
		this->push_back(new CDefinition(is));
		skipWhiteChars(is);
		//usun symbol ze strumienia tylko jesli jest to separator
		if (GetSymbol(is, false) == SymbolSeparator)
			GetSymbol(is, true);
		else
			break;
	} while (true);
	return is;
}

void CDefinitionList::WriteTo(std::ostream & os) const
{
	bool first = true;
	for (const CDefinition* def : *this)
		os << (first ? first = false, "" : "\n\t| ") << *def;
}

void CDefinitionList::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	for (const CDefinition* definition : *this)
		definition->ForEach(condition, action);
}

void CDefinitionList::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	for (CDefinition* definition : *this)
		definition->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CDefinitionList & list)
{
	list.WriteTo(os);
	return os;
}
