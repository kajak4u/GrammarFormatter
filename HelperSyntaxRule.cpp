#include "HelperSyntaxRule.h"
#include "Group.h"
#include "Term.h"
#include "Factor.h"
#include "ComplexDefinition.h"
#include "ShortDefinition.h"

using namespace std;

namespace GrammarSymbols
{

	CHelperSyntaxRule::~CHelperSyntaxRule()
	{
		if (originalRule)
			delete originalRule;
	}

	CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CDefinitionList& origin)
		: CSyntaxRule(helperIdentifier)
	{
		for (const IDefinition* item : origin)
			AddDefinition(dynamic_cast<IDefinition*>(item->spawn(true)));
	}

	CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier & helperIdentifier, const CMetaIdentifier & helperIdentifier2, Option option)
		: CSyntaxRule(helperIdentifier)
	{
		if (option == OptionOptional)
		{
			// H2 = H1 | [empty]
			CShortDefinition *shortDef = new CShortDefinition();
			shortDef->push_back(new CMetaIdentifier(helperIdentifier2));

			AddDefinition(nullptr);
			AddDefinition(shortDef);
		}
		else if (option == OptionRepetition)
		{
			// identifier, item | [empty] ?
			CShortDefinition* shortDef = new CShortDefinition();
			shortDef->push_back(new CMetaIdentifier(helperIdentifier2));
			shortDef->push_back(new CMetaIdentifier(helperIdentifier));
			AddDefinition(nullptr);
			AddDefinition(shortDef);
		}
	}

	void CHelperSyntaxRule::WriteTo(_STD ostream & os) const
	{
		if (originalRule != nullptr)
			os << GetIdentifier() << " = " << GetDefinitionList() << "; (* origin = " << *originalRule << " *)" << endl;
		else
			CSyntaxRule::WriteTo(os);
	}

	CGroup * CHelperSyntaxRule::CreateReplacement() const
	{
		CDefinitionList list;
		CComplexDefinition* definition = new CComplexDefinition();
		CFactor factor = CFactor(&GetIdentifier());
		definition->push_back(new CTerm(factor));
		list.push_back(definition);
		return new CGroup(list, OptionOptional);
	}
}