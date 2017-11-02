#include "Factor.h"
#include "Recognizer.h"
#include "main.h"
#include <string>
#include "Multiplier.h"

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
		mult->ReadFrom(is);
		multiplier = mult->GetValue();
		delete mult;
		if (GetSymbol(is, true) != SymbolRepetition)
			throw invalid_argument(string()+"Expected repetition symbol '*' after multiplier '"+to_string(multiplier)+"'.");
		obj = CRecognizer::CreateFor(is);
	}
	this->primary = dynamic_cast<CPrimary*>(obj);
	if (obj && !primary)
		throw invalid_argument(string() + "Expected primary");
	if(primary)
		this->primary->ReadFrom(is);
	return is;
}

void CFactor::WriteTo(std::ostream & os) const
{
	if (multiplier != 1)
		os << multiplier << " * ";
	if(primary)
		primary->WriteTo(os);
}

void CFactor::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	if(primary!=nullptr)
		primary->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CFactor & factor)
{
	factor.WriteTo(os);
	return os;
}
