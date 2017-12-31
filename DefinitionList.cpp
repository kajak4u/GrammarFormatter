#include "DefinitionList.h"
#include "main.h"
#include "ComplexDefinition.h"
#include "ShortDefinition.h"

namespace GrammarSymbols
{

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
		*((vector<IDefinition*>*)this) = _STD move((vector<IDefinition*>)other);
		other.clear();
		return *this;
	}

	_STD istream& CDefinitionList::ReadFrom(_STD istream& is)
	{
		do
		{
			this->push_back(new CComplexDefinition(is));
			skipWhiteChars(is);
			//usun symbol ze strumienia tylko jesli jest to separator
			if (GetSymbol(is, false) == SymbolSeparator)
				GetSymbol(is, true);
			else
				break;
		} while (true);
		return is;
	}

	void CDefinitionList::WriteTo(_STD ostream & os) const
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
			if (CComplexDefinition* complexDefinition = dynamic_cast<CComplexDefinition*>(definition))
			{
				definition = new CShortDefinition(complexDefinition);
				delete complexDefinition;
			}
		}

	}

	void CDefinitionList::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		CGrammarObject::ForEach(condition, action);
		for (const IDefinition* definition : *this)
			if(definition)
				definition->ForEach(condition, action);
	}

	void CDefinitionList::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		CGrammarObject::ForEach(condition, action);
		for (IDefinition* definition : *this)
			if(definition)
				definition->ForEach(condition, action);
	}

	_STD ostream & operator<<(_STD ostream & os, const CDefinitionList & list)
	{
		list.WriteTo(os);
		return os;
	}

}