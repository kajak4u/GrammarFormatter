#include "RuleGroup.h"

using namespace std;


CRuleGroup::~CRuleGroup()
{
	for (CRuleElement*& elem : content)
		delete elem;
}

std::istream& CRuleGroup::ReadFrom(std::istream& is)
{
	char c = is.get(), c2;
	char endch, endch2;
	switch (c)
	{
	case '[':
		endch = ']';
		options |= OptionOptional;
		break;
	case '{':
		endch = '}';
		options |= OptionRepetition;
		break;
	case '(':
		endch = ')';
		endch2 = c2 = is.get();
		switch (c2)
		{
		case '/':
			options |= OptionOptional;
			break;
		case ':':
			options |= OptionRepetition;
			break;
		default:
			endch2 = '\0';
			is.putback(c2);
		}
		break;
	default:
		throw invalid_argument(string()+"Group cannot begin with '"+c+"'");
		break;
	}
#pragma message("tu ciagle nic nie ma")
	return is;
}