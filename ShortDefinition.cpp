#include "ShortDefinition.h"
#include "ComplexDefinition.h"
#include "Terminal.h"
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
				throw MyException("Grammar exceptions not supported yet " __FILE__, __LINE__);
			const CFactor& factor = term->GetFactor();
			const CPrimary* primary = factor.GetPrimary();
			auto identifier = dynamic_cast<const CMetaIdentifier*>(primary);
			auto terminal = dynamic_cast<const CTerminal*>(primary);
			if (primary != nullptr && identifier == nullptr && terminal == nullptr)
				throw MyException("Expected terminal or identifier" __FILE__, __LINE__);
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


	void CShortDefinition::ForEach(function<bool(const CGrammarObject*)> condition, function<void(const CGrammarObject*)> action) const
	{
		CGrammarObject::ForEach(condition, action);
		for (const CPrimary* primary : *this)
			primary->ForEach(condition, action);
	}

	void CShortDefinition::ForEach(function<bool(const CGrammarObject*)> condition, function<void(CGrammarObject*)> action)
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
		cerr << "Get first from definition:";
		for (auto it = iter; it != end; ++it)
		{
			cerr << " ";
			(*it)->WriteTo(cerr);
		}
		cerr << endl;
		for (canBeEmpty = true; !(canBeEmpty = !canBeEmpty) && iter != end; ++iter)
		{
			CMetaIdentifier* nonterminal = dynamic_cast<CMetaIdentifier*>(*iter);
			if (nonterminal == nullptr)
			{
				cerr << "add terminal ";
				(*iter)->WriteTo(cerr);
				cerr << endl;
				res += dynamic_cast<CTerminal*>(*iter);
			}
			else if (canBeEmpty = nonterminal->First().Contains(nullptr))
			{
				cerr << "add without empty:";
				for (auto& elem : nonterminal->First())
					if (elem)
						cerr << " " << *elem;
				cerr << endl;
				res += (nonterminal->First() - nullptr);
			}
			else
			{
				cerr << "add fully:";
				for (auto& elem : nonterminal->First())
					cerr << " " << *elem;
				cerr << endl;
				res += nonterminal->First();
			}
		}
		if (!canBeEmpty)
		{
			cerr << "add also [empty]" << endl;
			res += nullptr;
		}
		return res;
	}
}