#include "Terminal.h"
#include "Recognizer.h"
#include "TerminalManager.h"
#include "MetaIdentifier.h"

using namespace std;

namespace GrammarSymbols
{
	CTerminal* CTerminal::unique = nullptr;

	CTerminal::CTerminal()
		: item(nullptr)
	{
	}

	CTerminal::CTerminal(const _STD string & value)
		: value(value), item(CTerminalManager::Register(value))
	{
	}

	CTerminal::CTerminal(_STD string && value)
		: value(value), item(CTerminalManager::Register(value))
	{
	}

	const _STD string & CTerminal::GetValue() const
	{
		return value;
	}

	CTerminal::~CTerminal()
	{
	}

	_STD istream& CTerminal::ReadFrom(_STD istream& is)
	{
		char c = is.get();
		if (c != '"' && c != '\'')
			throw invalid_argument(string() + "Terminal symbol should begin with single or double quote, '" + c + "' found instead.");
		getline(is, value, c);
		if (is.fail())
			throw invalid_argument(string() + "End-of-symbol character not found for terminal symbol '" + value + "'.");
		item = CTerminalManager::Register(value);
		return is;
	}

	ISpawnable * CTerminal::spawn(bool copy) const
	{
		return copy ? new CTerminal(*this) : new CTerminal();
	}

	void CTerminal::registerPrefixes()
	{
		CRecognizer::registerType(new CTerminal(), "'");
		CRecognizer::registerType(new CTerminal(), "\"");
	}

	CTerminal & CTerminal::operator=(const CTerminal & other)
	{
		value = other.value;
		return *this;
	}

	bool CTerminal::operator<(const CTerminal & other) const
	{
		return item < other.item;
	}

	void CTerminal::WriteTo(_STD ostream & os) const
	{
		os << "\"" << value << "\"";
	}

	CTerminal * CTerminal::Unique()
	{
		if (unique == nullptr)
		{
			auto& memory = CTerminalManager::GetMemory();
			int i = 1;
			while (memory.find(string(i, '$')) != memory.end())
				++i;
			unique = new CTerminal(string(i, '$'));
		}
		return unique;
	}

	CTerminal * CTerminal::Recognize(_STD istream& is)
	{
		char c = is.get();
		auto& memory = CTerminalManager::GetMemory();
		string name;
		while (memory.lower_bound(name+c) != memory.upper_bound(name+c+char(255)))
		{
			name += c;
			c = is.get();
		}
		is.putback(c);
		if(name.empty())
			return nullptr;
		return new CTerminal(name);
	}

	bool CTerminal::Equals(const CPrimary * other) const
	{
		const CTerminal* mi = dynamic_cast<const CTerminal*>(other);
		return mi != nullptr && mi->item == item;
	}

	int CTerminal::Compare(const CPrimary * other) const
	{
		if (const CTerminal* term = dynamic_cast<const CTerminal*>(other))
			return item-term->item;
		else if (is<const CDefinedGrammarSymbol*>(other))
			return -1;
		else
			return CPrimary::Compare(other);
	}

	_STD ostream & operator<<(_STD ostream & os, const CTerminal & terminal)
	{
		terminal.WriteTo(os);
		return os;
	}
}