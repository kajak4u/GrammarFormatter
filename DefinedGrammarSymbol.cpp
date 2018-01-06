#include "DefinedGrammarSymbol.h"
#include "DefinedSymbolManager.h"
#include "ShortDefinition.h"
#include "Terminal.h"

using namespace std;

namespace GrammarSymbols
{
	//mock class to provide access to additional data without accessing directly CDefinedSymbolManagerItem class
	class CMockDefinedGrammarSymbol : public CDefinedGrammarSymbol
	{
		string name;
	public:
		//constructor with name
		CMockDefinedGrammarSymbol(string name)
			: name(name) {
			Register();
		}
		//empty method for it must be implemented
		virtual std::istream & ReadFrom(std::istream & is) override
		{
			return is;
		}
		//empty method for it must be implemented
		virtual void WriteTo(std::ostream & os) const override
		{
		}
		//sample method for they must be implemented
		virtual ISpawnable * Spawn(bool copy = false) const override
		{
			return new CMockDefinedGrammarSymbol(*this);
		}
		//sample method for they must be implemented
		virtual bool Equals(const CPrimary * other) const override
		{
			return other==this;
		}
		//returns name
		virtual const std::string & GetName() const override
		{
			return name;
		}
	};


	CDefinedSymbolManagerItem * CDefinedGrammarSymbol::GetItem() const
	{
		return item;
	}
	void CDefinedGrammarSymbol::Register()
	{
		item = CDefinedSymbolManager::Register(GetName());
		++item->instances;
	}
	CDefinedGrammarSymbol::CDefinedGrammarSymbol()
	{
	}
	CDefinedGrammarSymbol::CDefinedGrammarSymbol(const CDefinedGrammarSymbol & other)
		: item(other.item)
	{
		if(item)
			++item->instances;
	}
	CDefinedGrammarSymbol::CDefinedGrammarSymbol(CDefinedGrammarSymbol && other)
		: item(other.item)
	{
		other.item = nullptr;
	}
	CDefinedGrammarSymbol::~CDefinedGrammarSymbol()
	{
		if (item)
			--item->instances;
	}
	CDefinedGrammarSymbol & CDefinedGrammarSymbol::operator=(const CDefinedGrammarSymbol & other)
	{
		if (item)
			--item->instances;
		item = other.item;
		if (item)
			++item->instances;
		return *this;
	}
	CDefinedGrammarSymbol & CDefinedGrammarSymbol::operator=(CDefinedGrammarSymbol && other)
	{
		swap(item, other.item);
		return *this;
	}
	MySet<CTerminal*, CompareObjects<CTerminal>>& CDefinedGrammarSymbol::First() const
	{
		return item->first;
	}

	bool CDefinedGrammarSymbol::TryAddFirstFrom(const CShortDefinition * def) const
	{
		//get terminals that can be at definition's begin
		MySet<CTerminal*, CompareObjects<CTerminal>> newSymbols = GetFirstFrom(def->begin(), def->end());
		//if nothing more than current FIRST subset, return false
		if (newSymbols.IsSubsetOf(First()))
			return false;
		//otherwise, add missing symbols and return true
		First() += newSymbols;
		return true;
	}

	MySet<CTerminal*, CompareObjects<CTerminal>>& CDefinedGrammarSymbol::Follow() const
	{
		return item->follow;
	}

	const MySet<const CDefinition*>& CDefinedGrammarSymbol::GetDefinitions() const
	{
		return item->definitions;
	}

	int CDefinedGrammarSymbol::Compare(const CPrimary * other) const
	{
		//if comparing with symbol, compare items' pointers
		if (const CDefinedGrammarSymbol* sp = dynamic_cast<const CDefinedGrammarSymbol*>(other))
			return item - sp->item;
		//if comparing with terminal, symbol is always greater
		else if (dynamic_cast<const CTerminal*>(other))
			return 1;
		//otherwise, use base class comparing method
		else
			return CPrimary::Compare(other);
	}

	void CDefinedGrammarSymbol::ForEach(std::function<void(const CDefinedGrammarSymbol*)> func)
	{
		auto& memory = CDefinedSymbolManager::GetMemory();
		for (auto& item : memory)
		{
			//provide an object to access additional memory without accessing directly CDefinedSymbolManagerItem
			CMockDefinedGrammarSymbol symbol(item.first);
			func(&symbol);
		}
	}

}