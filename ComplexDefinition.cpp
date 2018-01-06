#include "ComplexDefinition.h"
#include "MetaIdentifier.h"
#include "Term.h"

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
			os << (first ? first = false, "" : ", ") << term;
	}

	ISpawnable * CComplexDefinition::Spawn(bool copy) const
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
}