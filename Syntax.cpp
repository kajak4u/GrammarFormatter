#include "Syntax.h"
#include "SyntaxRule.h"
#include "main.h"

using namespace std;

CSyntax::CSyntax()
{
}


CSyntax::~CSyntax()
{
	for (CSyntaxRule*& rule : *this)
		delete rule;
}

istream & CSyntax::ReadFrom(istream & is)
{
	do
	{
		this->push_back(new CSyntaxRule(is));
		cout << "Read rule: " << *this->back() << endl;
		skipWhiteChars(is);
	} while (!is.eof());
	return is;
}

void CSyntax::WriteTo(std::ostream & os) const
{
	for (CSyntaxRule* rule : *this)
		os << *rule << endl;
}

std::ostream & operator<<(std::ostream & os, const CSyntax & syntax)
{
	syntax.WriteTo(os);
	return os;
}
