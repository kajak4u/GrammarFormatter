#include "Factor.h"
#include "main.h"
#include "Multiplier.h"
#include "Primary.h"
#include "Recognizer.h"
#include <string>

using namespace std;

namespace GrammarSymbols
{

	CFactor::CFactor()
	{
	}

	CFactor::CFactor(const CPrimary * primary, int multiplier)
		: multiplier(multiplier)
	{
		if (primary != nullptr)
			this->primary = dynamic_cast<CPrimary*>(primary->Spawn(true));
	}

	CFactor::CFactor(const CFactor & other)
		: multiplier(other.multiplier)
	{
		if (other.primary != nullptr)
			primary = dynamic_cast<CPrimary*>(other.primary->Spawn(true));
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

	void CFactor::SetPrimary(const CPrimary * newPrimary)
	{
		if (primary)
			delete primary;
		primary = dynamic_cast<CPrimary*>(newPrimary->Spawn(true));
	}


	CFactor::~CFactor()
	{
		if (primary)
			delete primary;
	}

	istream & CFactor::ReadFrom(istream & is)
	{
		ISpawnable* obj = CRecognizer::CreateFor(is);
		//if recognized multiplier - process it and recognize again
		if (CMultiplier* mult = dynamic_cast<CMultiplier*>(obj))
		{
			mult->ReadFrom(is);
			multiplier = mult->GetValue();
			delete mult;
			if (GetSymbol(is, true) != SymbolRepetition)
				throw invalid_argument(string() + "Expected repetition symbol '*' after multiplier '" + to_string(multiplier) + "'.");
			obj = CRecognizer::CreateFor(is);
		}
		this->primary = dynamic_cast<CPrimary*>(obj);
		if (obj && !primary)
			throw invalid_argument(string() + "Expected primary");
		if (primary)
			this->primary->ReadFrom(is);
		return is;
	}

	void CFactor::WriteTo(_STD ostream & os) const
	{
		if (multiplier != 1)
			os << multiplier << " * ";
		if (primary)
			primary->WriteTo(os);
	}

	void CFactor::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		//apply to itself...
		CGrammarObject::ForEach(condition, action);
		//...and to child primary
		if (primary != nullptr)
		{
			//workaround for ambiguous const and non-const call
			[condition, action](const CPrimary* primary) {primary->ForEach(condition, action); }(primary);
		}
	}

	void CFactor::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		//apply to itself...
		CGrammarObject::ForEach(condition, action);
		//...and to child primary
		if (primary != nullptr)
			primary->ForEach(condition, action);
	}
}