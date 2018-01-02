#pragma once

#include "Spawnable.h"
#include "Readable.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	//represents EBNF 'Multiplier', used only for input recognizing
	class CMultiplier : public ISpawnable, public IReadable, public CGrammarObject
	{
		//multiplier's value
		int value;
	public:
		//constructor
		CMultiplier();
		//destructor
		virtual ~CMultiplier();
		//returns mutltiplier's value
		int GetValue() const;
		//register its prefixes to CRecognizer
		static void RegisterPrefixes();

		//inherited from IReadable
		_STD istream & ReadFrom(_STD istream & is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from ISpawnable
		ISpawnable * Spawn(bool copy = false) const override;

	};
	//output stream operator
	_STD ostream& operator<<(_STD ostream& os, const CMultiplier& mult);
}