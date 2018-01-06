#include "DefinedSymbolManager.h"
#include "Recognizer.h"
#include "ShortDefinition.h"
#include "Special.h"
#include "Terminal.h"

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

	ISpawnable * CSpecial::Spawn(bool copy) const
	{
		return copy ? new CSpecial(*this) : new CSpecial();
	}

	void CSpecial::RegisterPrefixes()
	{
		CRecognizer::RegisterType(new CSpecial(), "?");
	}

	FormatEffect CSpecial::GetFormat() const
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
}