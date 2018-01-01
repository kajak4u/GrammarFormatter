#include "MetaIdentifier.h"
#include "Recognizer.h"
#include "Terminal.h"
#include "TerminalManager.h"

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
		//if unique does not exist, create it
		if (unique == nullptr)
		{
			//in case standard '$' is used, unique symbol will be sequence of '$', as long as needed
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
		//lower bound returns iterator to first element that is not lower than given
		//upper bound returns iterator to first element that is greater than given
		//so if they are equal, it means that both point to element greater then current, 
		//  that means prefix can no longer be found in memory cause last character does not fit
		while (memory.lower_bound(name+c) != memory.upper_bound(name+c+char(255)))
		{
			name += c;
			c = is.get();
		}
		//return last character back to stream, for it is not in this terminal
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
		//if comparing with terminal, then compare items' pointers
		if (const CTerminal* term = dynamic_cast<const CTerminal*>(other))
			return item-term->item;
		//if comparing with symbol, symbol is always greater
		else if (is<const CDefinedGrammarSymbol*>(other))
			return -1;
		//otherwise, use base class comparing method
		else
			return CPrimary::Compare(other);
	}

	_STD ostream & operator<<(_STD ostream & os, const CTerminal & terminal)
	{
		terminal.WriteTo(os);
		return os;
	}
}