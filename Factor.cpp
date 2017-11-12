#include "Factor.h"
#include "Recognizer.h"
#include "main.h"
#include <string>
#include "Multiplier.h"

using namespace std;

CFactor::CFactor()
{
}

CFactor::CFactor(const CPrimary * primary, int multiplier)
	: multiplier(multiplier)
{
	if (primary != nullptr)
		this->primary = dynamic_cast<CPrimary*>(primary->spawn(true));
}

CFactor::CFactor(const CFactor & other)
	: multiplier(other.multiplier)
{
	if(other.primary != nullptr)
		primary = dynamic_cast<CPrimary*>(other.primary->spawn(true));
}

CFactor::CFactor(CFactor && other)
	: primary(other.primary), multiplier(other.multiplier)
{
	other.primary = nullptr;
}

int CFactor::GetMultiplier() const
{
	return multiplier;
}

const CPrimary * CFactor::GetPrimary() const
{
	return primary;
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
	if (primary != nullptr)
	{
		//workaround for ambiguous const and non-const call
		[condition, action](const CPrimary* primary) {primary->ForEach(condition, action); }(primary);
	}
}

void CFactor::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	if (primary != nullptr)
		primary->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CFactor & factor)
{
	factor.WriteTo(os);
	return os;
}
