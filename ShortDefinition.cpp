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
		for (const CTerm* term : *previous)
		{
			if (term->HasException())
				throw MYEXCEPTION("Grammar exceptions not supported yet...", 1);
			const CFactor& factor = term->GetFactor();
			const CPrimary* primary = factor.GetPrimary();
			auto identifier = dynamic_cast<const CMetaIdentifier*>(primary);
			auto terminal = dynamic_cast<const CTerminal*>(primary);
			auto special = dynamic_cast<const CSpecial*>(primary);
			if (primary != nullptr && identifier == nullptr && terminal == nullptr && special == nullptr)
				throw MYEXCEPTION("Expected terminal, identifier or special", -1);
			else if (identifier)
				identifier->MarkAsUsed();
			for (int i = 0; i < factor.GetMultiplier(); ++i)
			{
				if(primary!=nullptr)
					push_back(dynamic_cast<CPrimary*>(primary->spawn(true)));
			}
		}
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

			skipWhiteChars(is);
			//usun symbol ze strumienia tylko jesli jest to separator
			if (GetSymbol(is, false) == SymbolConcatenate)
				GetSymbol(is, true);
			else
				break;
		} while (true);
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
		//loop until encounters a nonterminal without [empty] or a terminal
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