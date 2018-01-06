#include "DefinedSymbolManager.h"
#include "MetaIdentifier.h"
#include "Recognizer.h"

using namespace std;

namespace GrammarSymbols
{

	CMetaIdentifier::CMetaIdentifier(const _STD string & name)
		: name(name)
	{
		Register();
	}

	CMetaIdentifier::CMetaIdentifier(_STD string && name)
		: name(name)
	{
		Register();
	}

	CMetaIdentifier::CMetaIdentifier()
	{
	}


	CMetaIdentifier::~CMetaIdentifier()
	{
	}

	const _STD string & CMetaIdentifier::GetName() const
	{
		return name;
	}

	_STD istream & CMetaIdentifier::ReadFrom(_STD istream & is)
	{
		SkipWhiteChars(is);
		char c = is.get();
		if (!isalpha(c))
			throw invalid_argument(string() + "Meta identifier should start with letter, '" + c + "' found instead.");
		//add all valid characters to identifier's name
		do
		{
			name += c;
			c = is.get();
		} while (isalnum(c) || c == ' ' || c == '_');
		//pop all trailing spaces
		while (name.back() == ' ')
			name.pop_back();
		Register();
		return is.putback(c);
	}

	void CMetaIdentifier::WriteTo(_STD ostream & os) const
	{
		os << &name;
	}

	ISpawnable * CMetaIdentifier::Spawn(bool copy) const
	{
		return copy ? new CMetaIdentifier(*this) : new CMetaIdentifier();
	}

	void CMetaIdentifier::RegisterPrefixes()
	{
		//register all small and capital letters
		for (char c = 'a'; c <= 'z'; ++c)
		{
			CRecognizer::RegisterType(new CMetaIdentifier(), string{ c });
			CRecognizer::RegisterType(new CMetaIdentifier(), string{ (char)toupper(c) });
		}
	}

	bool CMetaIdentifier::operator<(const CMetaIdentifier & other) const
	{
		return GetItem() < other.GetItem();
	}

	void CMetaIdentifier::MarkAsDefinedBy(const CDefinition* def) const
	{
		GetItem()->definitions.insert(def);
	}

	void CMetaIdentifier::MarkAsUsed() const
	{
		GetItem()->used = true;
	}

	bool CMetaIdentifier::GetWarnings(MySet<_STD string>& undefined, MySet<_STD string>& unused)
	{
		auto& memory = CDefinedSymbolManager::GetMemory();
		for (auto& keyVal : memory)
		{
			//add to unused if is not marked as used
			if (!keyVal.second->used)
				unused.insert(keyVal.first);
			//add to undefined if has 0 definitions
			if (keyVal.second->definitions.empty())
				undefined.insert(keyVal.first);
		}
		// starting symbol should be unused, so only valid case is: 0 undefined and 1 unused
		return !undefined.empty() || unused.size() != 1;
	}

	bool CMetaIdentifier::Equals(const CPrimary * other) const
	{
		const CMetaIdentifier* mi = dynamic_cast<const CMetaIdentifier*>(other);
		return mi != nullptr && mi->GetItem() == GetItem();
	}
}