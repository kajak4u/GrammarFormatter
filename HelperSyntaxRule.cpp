#include "HelperSyntaxRule.h"
#include "Group.h"
#include "Term.h"
#include "Factor.h"
#include "Definition.h"

using namespace std;


CHelperSyntaxRule::~CHelperSyntaxRule()
{
	if (originalRule)
		delete originalRule;
}

CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CGroup * origin)
	: CSyntaxRule(helperIdentifier)
{
	originalRule = new CGroup(*origin);
	CGroup *groupWithoutRepetition = new CGroup(origin->getDefinitionList(), OptionNone);
	CTerm *term = new CTerm(CFactor(groupWithoutRepetition));
	CTerm *identifierTerm = new CTerm(CFactor(&helperIdentifier));

	CDefinition* definition = new CDefinition();
	definition->push_back(term); // H1 -> a
	AddCopyDefinition(definition);
	definition->push_back(identifierTerm);
	AddDefinition(definition); // H1 -> a,H1
}

void CHelperSyntaxRule::WriteTo(std::ostream & os) const
{
	os << GetIdentifier() << " = " << GetDefinitionList() << "; (* origin = "<< *originalRule <<" *)" << endl;
}

CGroup * CHelperSyntaxRule::CreateReplacement() const
{
	CDefinitionList list;
	CDefinition* definition = new CDefinition();
	CFactor factor = CFactor(&GetIdentifier());
	definition->push_back(new CTerm(factor));
	list.push_back(definition);
	return new CGroup(list, OptionOptional);
}
