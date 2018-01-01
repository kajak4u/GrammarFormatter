#include "ShortDefinition.h"
#include "ComplexDefinition.h"
#include "Terminal.h"
#include "Special.h"
#include "Term.h"
#include "main.h"
#include "MetaIdentifier.h"
#include "Recognizer.h"

using namespace std;

namespace GrammarSymbols
{
	CShortDefinition::CShortDefinition()
	{
	}

	CShortDefinition::CShortDefinition(const CShortDefinition & other)
	{
		reserve(other.size());
		for (const CPrimary* primary : other)
			push_back(dynamic_cast<CPrimary*>(primary->spawn(true)));
	}

	CShortDefinition::CShortDefinition(CComplexDefinition * previous)
	{
		//convert terms to primary objects
		//expects that there are no group objects already
		for (const CTerm* term : *previous)
		{
			//todo in the future...
			if (term->HasException())
				throw MYEXCEPTION("Exception clauses not supported yet...", 1);
			const CFactor& factor = term->GetFactor();
			const CPrimary* primary = factor.GetPrimary();
			if (primary == nullptr)
				continue;
			if (primary != nullptr && !is<const CDefinedGrammarSymbol*>(primary) && !is<const CTerminal*>(primary))
				throw MYEXCEPTION("Expected terminal, identifier or special", -1);
			else if (auto identifier = dynamic_cast<const CMetaIdentifier*>(primary))
				identifier->MarkAsUsed();
			//get rid of multiplier by simply multiplying corresponding primary
			for (int i = 0; i < factor.GetMultiplier(); ++i)
				push_back(dynamic_cast<CPrimary*>(primary->spawn(true)));
		}
	}

	CShortDefinition::CShortDefinition(std::initializer_list<CPrimary*> list)
		: vector<CPrimary*>(list)
	{
	}

	CShortDefinition::~CShortDefinition()
	{
		for (CPrimary* primary : *this)
			delete primary;
	}

	istream & CShortDefinition::ReadFrom(istream & is)
	{
		do
		{
			ISpawnable* obj = CRecognizer::CreateFor(is);
			CPrimary* primary = dynamic_cast<CPrimary*>(obj);
			if (obj && !primary)
				throw invalid_argument(string() + "Expected primary");
			if (primary)
				primary->ReadFrom(is);
			this->push_back(primary);
			//if next symbol is concatenate - it is followed by next definition's symbol
		} while (TakeSymbolIf(is, SymbolConcatenate));
		return is;
	}

	void CShortDefinition::WriteTo(_STD ostream & os) const
	{
		bool first = true;
		for (const CPrimary* primary : *this)
		{
			os << (first ? first = false, "" : ", ");
			if (primary != nullptr)
				primary->WriteTo(os);
			else
				os << "[empty]";
		}
	}

	ISpawnable * CShortDefinition::spawn(bool copy) const
	{
		return copy ? new CShortDefinition(*this) : new CShortDefinition();
	}


	void CShortDefinition::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		CGrammarObject::ForEach(condition, action);
		for (const CPrimary* primary : *this)
			primary->ForEach(condition, action);
	}

	void CShortDefinition::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		CGrammarObject::ForEach(condition, action);
		for (CPrimary* primary : *this)
			primary->ForEach(condition, action);
	}


	MySet<CTerminal*, CompareObjects<CTerminal>> GetFirstFrom(_STD vector<CPrimary*>::const_iterator iter, _STD vector<CPrimary*>::const_iterator end)
	{
		MySet<CTerminal*, CompareObjects<CTerminal>> res;
		//loop until encounters either end or a terminal or a nonterminal that cannot be derivated from empty symbol
		bool canBeEmpty;
#ifdef DEBUG_PARSINGTABLE
		cerr << "Get first from definition:";
		for (auto it = iter; it != end; ++it)
		{
			cerr << " ";
			(*it)->WriteTo(cerr);
		}
		cerr << endl;
#endif
		for (canBeEmpty = true; !(canBeEmpty = !canBeEmpty) && iter != end; ++iter)
		{
			CDefinedGrammarSymbol* nonterminal = dynamic_cast<CDefinedGrammarSymbol*>(*iter);
			CTerminal* terminal = dynamic_cast<CTerminal*>(*iter);
			if (terminal != nullptr)
			{
#ifdef DEBUG_PARSINGTABLE
				cerr << "add terminal ";
				(*iter)->WriteTo(cerr);
				cerr << endl;
#endif
				res += terminal;
			}
			else if (canBeEmpty = nonterminal->First().Contains(nullptr))
			{
#ifdef DEBUG_PARSINGTABLE
				cerr << "add without empty:";
				for (auto& elem : nonterminal->First())
					if (elem)
						cerr << " " << *elem;
				cerr << endl;
#endif
				res += (nonterminal->First() - nullptr);
			}
			else
			{
#ifdef DEBUG_PARSINGTABLE
				cerr << "add fully:";
				for (auto& elem : nonterminal->First())
					cerr << " " << *elem;
				cerr << endl;
#endif
				res += nonterminal->First();
			}
		}
		//if reached end, FIRST set should also contain empty symbol
		if (!canBeEmpty)
		{
#ifdef DEBUG_PARSINGTABLE
			cerr << "add also [empty]" << endl;
#endif
			res += nullptr;
		}
		return res;
	}
}