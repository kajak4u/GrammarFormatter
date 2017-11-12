#include "Terminal.h"
#include "Recognizer.h"

using namespace std;

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

void CTerminal::WriteTo(std::ostream & os) const
{
	os << "\"" << value << "\"";
}

std::ostream & operator<<(std::ostream & os, const CTerminal & terminal)
{
	terminal.WriteTo(os);
	return os;
}
