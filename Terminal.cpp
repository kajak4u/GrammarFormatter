#include "Terminal.h"
#include "Recognizer.h"

using namespace std;

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

ISpawnable * CTerminal::spawn() const
{
	return new CTerminal();
}

void CTerminal::registerPrefixes()
{
	CRecognizer::registerType(new CTerminal(), "'");
	CRecognizer::registerType(new CTerminal(), "\"");
}