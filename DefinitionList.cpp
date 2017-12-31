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
		for (CDefinition* definition : other)
			this->push_back(dynamic_cast<CDefinition*>(definition->spawn(true)));
	}


	CDefinitionList::~CDefinitionList()
	{
		for (CDefinition*& def : *this)
			if(def)
				delete def;
	}

	CDefinitionList CDefinitionList::operator=(CDefinitionList && other)
	{
		*((vector<CDefinition*>*)this) = _STD move((vector<CDefinition*>)other);
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
		for (const CDefinition* def : *this)
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
		for (CDefinition*& definition : *this)
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
		for (const CDefinition* definition : *this)
			if(definition)
				definition->ForEach(condition, action);
	}

	void CDefinitionList::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		CGrammarObject::ForEach(condition, action);
		for (CDefinition* definition : *this)
			if(definition)
				definition->ForEach(condition, action);
	}

	_STD ostream & operator<<(_STD ostream & os, const CDefinitionList & list)
	{
		list.WriteTo(os);
		return os;
	}

}