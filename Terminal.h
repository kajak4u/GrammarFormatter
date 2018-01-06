#pragma once

#include <string>
#include "Primary.h"

namespace GrammarSymbols
{
	class CTerminalManagerItem;

	//represents EBNF 'Terminal'
	class CTerminal : public CPrimary
	{
		//unique terminal, unused in syntax
		static CTerminal* unique;
		//value
		_STD string value;
		//corresponding global data
		CTerminalManagerItem* item;
	public:
		//empty constructor
		CTerminal();
		//constructor copying value
		CTerminal(const _STD string& value);
		//constructor moving value
		CTerminal(_STD string&& value);
		//destructor
		virtual ~CTerminal();
		//returns value
		const _STD string& GetValue() const;
		//register its prefixes to CRecognizer
		static void RegisterPrefixes();
		//assign operator
		CTerminal& operator=(const CTerminal& other);
		//relation operator
		bool operator<(const CTerminal& other) const;
		//creates or returns unique terminal
		static CTerminal* Unique();
		//recognizes terminal corresponding to input stream
		static CTerminal* Recognize(_STD istream& is);
		
		//inherited from IReadable
		_STD istream& ReadFrom(_STD istream& is) override;
		void WriteTo(_STD ostream & os) const override;
		//inherited from ISpawnable
		ISpawnable * Spawn(bool copy = false) const override;
		//inherited from CPrimary
		bool Equals(const CPrimary * other) const override;
		int Compare(const CPrimary* other) const override;
	};
}