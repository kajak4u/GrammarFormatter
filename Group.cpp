#include "Group.h"
#include "main.h"
#include "Recognizer.h"

using namespace std;

namespace GrammarSymbols
{

	GroupType CGroup::GetType() const
	{
		return type;
	}

	CGroup::CGroup()
	{
	}

	CGroup::CGroup(const CDefinitionList & list, GroupType type)
		: definitionList(list), type(type)
	{
	}

	CGroup::CGroup(GroupType type)
		: type(type)
	{
	}

	CGroup::~CGroup()
	{
	}

	_STD istream& CGroup::ReadFrom(_STD istream& is)
	{
		char c = is.get(), c2;
		char endch, endch2 = '\0';
		//recognize group type based on opening sequence
		switch (c)
		{
		case '[':
			endch = ']';
			type = GroupOptional;
			break;
		case '{':
			endch = '}';
			type = GroupRepetition;
			break;
		case '(':
			endch2 = ')';
			endch = c2 = is.get();
			//check two-characters sequences: (/ for [ and (: for {
			switch (c2)
			{
			case '/':
				type = GroupOptional;
				break;
			case ':':
				type = GroupRepetition;
				break;
			default:
				endch = endch2;
				endch2 = '\0';
				is.putback(c2);
			}
			break;
		default:
			throw invalid_argument(string() + "Group cannot begin with '" + c + "'");
			break;
		}
		//read corresponding definition list
		is >> definitionList;
		SkipWhiteChars(is);
		//expect proper ending
		c = is.get();
		if (c != endch)
			throw invalid_argument(string() + "Expected end-of-group symbol '" + endch + "', got '" + c + "'");
		if (endch2 != '\0' && (c = is.get()) != endch2)
			throw invalid_argument(string() + "Expected end-of-group symbol '" + endch + endch2 + "', got '" + endch + c + "'");

		return is;
	}

	const CDefinitionList & CGroup::GetDefinitionList() const
	{
		return definitionList;
	}

	ISpawnable* CGroup::Spawn(bool copy) const
	{
		return copy ? new CGroup(*this) : new CGroup();
	}

	void CGroup::RegisterPrefixes()
	{
		CRecognizer::RegisterType(new CGroup(), "[");
		CRecognizer::RegisterType(new CGroup(), "(/");

		CRecognizer::RegisterType(new CGroup(), "{");
		CRecognizer::RegisterType(new CGroup(), "(:");

		CRecognizer::RegisterType(new CGroup(), "(");
	}

	void CGroup::WriteTo(_STD ostream & os) const
	{
		string brackets = type == GroupOptional ? "[]" : type == GroupRepetition ? "{}" : "()";
		os << brackets[0] << definitionList << brackets[1];
	}


	void CGroup::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		//apply to itself...
		CGrammarObject::ForEach(condition, action);
		//...and to definition list
		definitionList.ForEach(condition, action);
	}

	void CGroup::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
	{
		//apply to itself...
		CGrammarObject::ForEach(condition, action);
		//...and to definition list
		definitionList.ForEach(condition, action);
	}

	bool CGroup::Equals(const CPrimary * other) const
	{
		const CGroup* mi = dynamic_cast<const CGroup*>(other);
		return mi != nullptr && mi->type == type && mi->definitionList == definitionList;
	}
}