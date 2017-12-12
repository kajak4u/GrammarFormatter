#pragma once

#include <string>
#include "Primary.h"
#include "GrammarObject.h"

namespace GrammarSymbols
{
	class CTerminalManagerItem;

	class CTerminal : public CPrimary
	{
		_STD string value;
		CTerminalManagerItem* item;
		static CTerminal* unique;
	public:
		CTerminal();
		CTerminal(const _STD string& value);
		CTerminal(_STD string&& value);
		const _STD string& GetValue() const;
		virtual ~CTerminal();
		_STD istream& ReadFrom(_STD istream& is) override;
		ISpawnable * spawn(bool copy = false) const override;
		static void registerPrefixes();
		CTerminal& operator=(const CTerminal& other);

		bool operator<(const CTerminal& other) const;

		void WriteTo(_STD ostream & os) const override;
		bool Equals(const CPrimary * other) const override;
		int Compare(const CPrimary* other) const override;
		static CTerminal* Unique();
		static CTerminal* Recognize(_STD istream& is);
	};
	_STD ostream& operator<<(_STD ostream& os, const CTerminal& terminal);
}