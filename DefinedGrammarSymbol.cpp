#include "DefinedGrammarSymbol.h"
#include "MetaIdentifierManager.h"
#include "Terminal.h"
#include "ShortDefinition.h"

using namespace std;

namespace GrammarSymbols
{
	class CMockDefinedGrammarSymbol : public CDefinedGrammarSymbol
	{
		string name;
	public:
		CMockDefinedGrammarSymbol(string name)
			: name(name) {
			Register();
		}
		virtual std::istream & ReadFrom(std::istream & is) override
		{
			return is;
		}
		virtual void WriteTo(std::ostream & os) const override
		{
		}
		virtual ISpawnable * spawn(bool copy = false) const override
		{
			return new CMockDefinedGrammarSymbol(*this);
		}
		virtual bool Equals(const CPrimary * other) const override
		{
			return false;
		}
		virtual const std::string & GetName() const override
		{
			return name;
		}
	};


	CMetaIdentifierManagerItem * CDefinedGrammarSymbol::GetItem() const
	{
		return item;
	}
	void CDefinedGrammarSymbol::Register()
	{
		item = CMetaIdentifierManager::Register(GetName());
	}
	MySet<CTerminal*, CompareObjects<CTerminal>>& CDefinedGrammarSymbol::First() const
	{
		return item->first;
	}

	bool CDefinedGrammarSymbol::TryAddFirstFrom(const CShortDefinition * def) const
	{
		MySet<CTerminal*, CompareObjects<CTerminal>> newSymbols = GetFirstFrom(def->begin(), def->end());
		if (newSymbols.IsSubsetOf(First()))
			return false;
		First() += newSymbols;
		return true;
	}

	MySet<CTerminal*, CompareObjects<CTerminal>>& CDefinedGrammarSymbol::Follow() const
	{
		return item->follow;
	}

	const MySet<const IDefinition*>& CDefinedGrammarSymbol::GetDefinitions() const
	{
		return item->definitions;
	}

	int CDefinedGrammarSymbol::Compare(const CPrimary * other) const
	{
		if (const CDefinedGrammarSymbol* sp = dynamic_cast<const CDefinedGrammarSymbol*>(other))
			return item - sp->item;
		else if (dynamic_cast<const CTerminal*>(other))
			return 1;
		else
			return CPrimary::Compare(other);
	}

	void CDefinedGrammarSymbol::ForEach(std::function<void(const CDefinedGrammarSymbol*)> func)
	{
		auto& memory = CMetaIdentifierManager::GetMemory();
		for (auto& item : memory)
		{
			CMockDefinedGrammarSymbol symbol(item.first);
			func(&symbol);
		}
	}

	std::ostream & operator<<(std::ostream & os, const CDefinedGrammarSymbol & identifier)
	{
		identifier.WriteTo(os);
		return os;
	}

}