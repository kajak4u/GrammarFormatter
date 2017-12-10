#include "Terminal.h"
#include "Recognizer.h"
#include "TerminalManager.h"

using namespace std;

CTerminal::CTerminal()
	: item(nullptr)
{
}

CTerminal::CTerminal(const std::string & value)
	:value(value), item(CTerminalManager::Register(value))
{
}

CTerminal::CTerminal(std::string && value)
	: value(value), item(CTerminalManager::Register(value))
{
}

const std::string & CTerminal::GetValue() const
{
	return value;
}

CTerminal::~CTerminal()
{
}

std::istream& CTerminal::ReadFrom(std::istream& is)
{
	char c = is.get();
	if (c != '"' && c!='\'')
		throw invalid_argument(string()+"Terminal symbol should begin with single or double quote, '" + c + "' found instead.");
	getline(is, value, c);
	if (is.fail())
		throw invalid_argument(string() + "End-of-symbol character not found for terminal symbol '"+value+"'.");
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

void CTerminal::WriteTo(std::ostream & os) const
{
	os << "\"" << value << "\"";
}

CTerminal * CTerminal::CreateUnique()
{
	auto& memory = CTerminalManager::GetMemory();
	int i=1;
	while (memory.find(string(i, '$'))!=memory.end())
		++i;
	return new CTerminal(string(i, '$'));
}

bool CTerminal::Equals(const CPrimary * other) const
{
	const CTerminal* mi = dynamic_cast<const CTerminal*>(other);
	return mi != nullptr && mi->item == item;
}

std::ostream & operator<<(std::ostream & os, const CTerminal & terminal)
{
	terminal.WriteTo(os);
	return os;
}
