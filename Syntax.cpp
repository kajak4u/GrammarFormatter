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
		skipWhiteChars(is);
	} while (!is.eof());
	return is;
}
