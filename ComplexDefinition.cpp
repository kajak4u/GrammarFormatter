#include "ComplexDefinition.h"
#include "Enums.h"
#include "main.h"
#include "Term.h"
#include "MetaIdentifier.h"
#include "Terminal.h"
#include "Recognizer.h"

using namespace std;

namespace GrammarSymbols
{

	CComplexDefinition::CComplexDefinition()
	{
	}

	CComplexDefinition::CComplexDefinition(_STD istream & is)
		: CComplexDefinition()
	{
		ReadFrom(is);
	}

	CComplexDefinition::CComplexDefinition(const CComplexDefinition & other)
	{
		this->reserve(other.size());
		for (CTerm* term : other)
			this->push_back(new CTerm(*term));
	}


	CComplexDefinition::~CComplexDefinition()
	{
		for (CTerm*& term : *this)
			delete term;
	}

	istream& CComplexDefinition::ReadFrom(istream& is)
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

	void CComplexDefinition::WriteTo(ostream & os) const
	{
		bool first = true;
		for (const CTerm* term : *this)
			os << (first ? first = false, "" : ", ") << *term;
	}

	ISpawnable * CComplexDefinition::spawn(bool copy) const
	{
		return copy ? new CComplexDefinition(*this) : new CComplexDefinition();
	}

	void CComplexDefinition::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		CGrammarObject::ForEach(condition, action);
		for (const CTerm* term : *this)
			term->ForEach(condition, action);
	}

	void CComplexDefinition::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		CGrammarObject::ForEach(condition, action);
		for (CTerm* term : *this)
			term->ForEach(condition, action);
	}

	ostream & operator<<(ostream & os, const CComplexDefinition & def)
	{
		def.WriteTo(os);
		return os;
	}
}