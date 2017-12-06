#include "DefinitionList.h"
#include "main.h"
#include "Definition.h"


CDefinitionList::CDefinitionList()
{
}

CDefinitionList::CDefinitionList(const CDefinitionList & other)
{
	this->reserve(other.size());
	for (IDefinition* definition : other)
		this->push_back(dynamic_cast<IDefinition*>(definition->spawn(true)));
}


CDefinitionList::~CDefinitionList()
{
	for (IDefinition*& def : *this)
		if(def)
			delete def;
}

CDefinitionList CDefinitionList::operator=(CDefinitionList && other)
{
	*((vector<IDefinition*>*)this) = std::move((vector<IDefinition*>)other);
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
	for (const IDefinition* def : *this)
	{
		os << (first ? first = false, "" : "\n\t| ");
		if (def)
			def->WriteTo(os);
		else
			os << "[empty]";
	}
}

void CDefinitionList::Simplify()
{
	for (IDefinition*& definition : *this)
	{
		if (CDefinition* complexDefinition = dynamic_cast<CDefinition*>(definition))
		{
			definition = new CShortDefinition(complexDefinition);
			delete complexDefinition;
		}
	}

}

void CDefinitionList::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	for (const IDefinition* definition : *this)
		if(definition)
			definition->ForEach(condition, action);
}

void CDefinitionList::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	for (IDefinition* definition : *this)
		if(definition)
			definition->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CDefinitionList & list)
{
	list.WriteTo(os);
	return os;
}
