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
			//if next symbol is concatenate - it is followed by next definition's symbol
		} while (TakeSymbolIf(is, SymbolConcatenate));
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
		//apply to itself...
		CGrammarObject::ForEach(condition, action);
		//...and to every child term
		for (const CTerm* term : *this)
			term->ForEach(condition, action);
	}

	void CComplexDefinition::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		//apply to itself...
		CGrammarObject::ForEach(condition, action);
		//...and to every child term
		for (CTerm* term : *this)
			term->ForEach(condition, action);
	}

	ostream & operator<<(ostream & os, const CComplexDefinition & def)
	{
		def.WriteTo(os);
		return os;
	}
}