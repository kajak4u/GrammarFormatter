#pragma once

#include "Readable.h"
#include "Spawnable.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	//abstract class, represents EBNF 'Primary'
	class CPrimary : public IReadable, public ISpawnable, public CGrammarObject
	{
	public:
		//returns whether object is equal to given
		virtual bool Equals(const CPrimary* other) const = 0;
		//returns order with another object: >0 if greater, 0 if equal or <0 if lower
		virtual int Compare(const CPrimary* other) const;
		//returns whether object is lower than given
		virtual bool operator<(const CPrimary& other) const;
		//constructor
		CPrimary();
		//destructor
		virtual ~CPrimary();
	};

}