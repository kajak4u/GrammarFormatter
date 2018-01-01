#include "Special.h"
#include "Recognizer.h"
#include "main.h"
#include <string>
#include "MetaIdentifier.h"
#include "Terminal.h"
#include "ShortDefinition.h"
#include "DefinedSymbolManager.h"

using namespace std;

namespace GrammarSymbols
{
	map<string, FormatEffect> CSpecial::formatMap = {
		{ "NL",		FormatNewLine },
		{ "NEWLINE",FormatNewLine },
		{ "INT",	FormatIntend },
		{ "INTEND", FormatIntend },
		{ "DED",	FormatDedend },
		{ "DEDEND", FormatDedend },
		{ "SPACE",	FormatSpace },
		{ "NOSPACE", FormatNoSpace},
		{ "TAB",	FormatTab }
	};

	CSpecial::CSpecial()
	{
	}


	CSpecial::~CSpecial()
	{
		//if removing last instance, remove its corresponding definitions
		auto item = GetItem();
		if (item != nullptr && item->instances == 1)
		{
			for (auto& definition : item->definitions)
				delete definition;
			item->definitions.clear();
		}
	}

	_STD istream & CSpecial::ReadFrom(_STD istream & is)
	{
		SkipWhiteChars(is);
		char c = is.get();
		if (c != '?')
			throw invalid_argument(string() + "Special sequence should start with question mark, '" + c + "' found instead.");
		SkipWhiteChars(is);
		//add characters until encounters another question mark
		while (!is.eof() && (c = is.get()) != '?')
			name += toupper(c);
		if (c != '?')
			throw invalid_argument(string() + "Special sequence should end with question mark, '" + c + "' found instead.");
		//pop back trailing spaces
		while (name.back() == ' ')
			name.pop_back();
		//assign corresponding format
		auto iter = formatMap.find(name);
		if (iter != formatMap.end())
			format = iter->second;
		Register();
		//if first defined, add empty definition
		const auto& item = GetItem();
		item->used = true;
		if (item->definitions.empty())
			item->definitions.insert(new CShortDefinition());
		return is;
	}

	void CSpecial::WriteTo(_STD ostream & os) const
	{
		os << "?" << name << "?";
	}

	ISpawnable * CSpecial::spawn(bool copy) const
	{
		return copy ? new CSpecial(*this) : new CSpecial();
	}

	void CSpecial::registerPrefixes()
	{
		CRecognizer::registerType(new CSpecial(), "?");
	}

	FormatEffect CSpecial::getFormat() const
	{
		return format;
	}

	const std::string & CSpecial::GetName() const
	{
		return name;
	}

	bool CSpecial::Equals(const CPrimary * other) const
	{
		const CSpecial* mi = dynamic_cast<const CSpecial*>(other);
		return mi != nullptr && mi->name == name;
	}

	_STD ostream & operator<<(_STD ostream & os, const CSpecial & special)
	{
		special.WriteTo(os);
		return os;
	}
}