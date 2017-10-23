#include "Factor.h"
#include "Recognizer.h"
#include "main.h"
#include <string>

using namespace std;

CFactor::CFactor()
{
}


CFactor::~CFactor()
{
	if (primary)
		delete primary;
}

istream & CFactor::ReadFrom(istream & is)
{
	ISpawnable* obj = CRecognizer::CreateFor(is);
	if (CMultiplier* mult = dynamic_cast<CMultiplier*>(obj))
	{
		multiplier = mult->value;
		delete mult;
		if (GetSymbol(is, true) != SymbolRepetition)
			throw invalid_argument(string()+"Expected repetition symbol '*' after multiplier '"+to_string(multiplier)+"'.");
		obj = CRecognizer::CreateFor(is);
	}
	this->primary = dynamic_cast<CPrimary*>(obj);
	if (obj && !primary)
		throw invalid_argument(string() + "Expected primary");

	return is;
}
