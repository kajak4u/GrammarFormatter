#include "MetaIdentifier.h"
#include "Recognizer.h"
#include "main.h"
#include <string>
#include "MetaIdentifierManager.h"
#include "ShortDefinition.h"
#include "Terminal.h"
#include "Special.h"

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
		skipWhiteChars(is);
		char c = is.get();
		if (!isalpha(c))
			throw invalid_argument(string() + "Meta identifier should start with letter, '" + c + "' found instead.");

		do
		{
			name += c;
			c = is.get();
		} while (isalnum(c) || c == ' ' || c=='_'/*|| c=='-'*/);
		while (name.back() == ' ')
			name.pop_back();
		Register();
		return is.putback(c);
	}

	void CMetaIdentifier::WriteTo(_STD ostream & os) const
	{
		os << name;
	}

	ISpawnable * CMetaIdentifier::spawn(bool copy) const
	{
		return copy ? new CMetaIdentifier(*this) : new CMetaIdentifier();
	}

	void CMetaIdentifier::registerPrefixes()
	{
		for (char c = 'a'; c <= 'z'; ++c)
		{
			CRecognizer::registerType(new CMetaIdentifier(), string{ c });
			CRecognizer::registerType(new CMetaIdentifier(), string{ (char)toupper(c) });
		}
	}

	bool CMetaIdentifier::operator<(const CMetaIdentifier & other) const
	{
		return GetItem() < other.GetItem();
	}

	void CMetaIdentifier::MarkAsDefinedBy(const IDefinition* def) const
	{
		GetItem()->definitions.insert(def);
	}

	void CMetaIdentifier::MarkAsUsed() const
	{
		GetItem()->used = true;
	}

	bool CMetaIdentifier::GetWarnings(MySet<_STD string>& undefined, MySet<_STD string>& unused)
	{
		auto& memory = CMetaIdentifierManager::GetMemory();
		for (auto& keyVal : memory)
		{
			if (!keyVal.second->used)
				unused.insert(keyVal.first);
			if (keyVal.second->definitions.empty())
				undefined.insert(keyVal.first);
		}
		return !undefined.empty() || unused.size() != 1; // starting symbol is unused
	}

	bool CMetaIdentifier::Equals(const CPrimary * other) const
	{
		const CMetaIdentifier* mi = dynamic_cast<const CMetaIdentifier*>(other);
		return mi != nullptr && mi->GetItem() == GetItem();
	}

	_STD ostream & operator<<(_STD ostream & os, const CMetaIdentifier & identifier)
	{
		identifier.WriteTo(os);
		return os;
	}
}