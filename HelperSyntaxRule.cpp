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
	}

	CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier& helperIdentifier, const CDefinitionList& origin)
		: CSyntaxRule(helperIdentifier)
	{
		for (const CDefinition* item : origin)
			AddDefinition(dynamic_cast<CDefinition*>(item->spawn(true)));
	}

	CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier & helperIdentifier, const CMetaIdentifier & helperIdentifier2, GroupType option)
		: CSyntaxRule(helperIdentifier)
	{
		if (option == GroupOptional)
		{
			// H2 = H1 | [empty]
			CShortDefinition *shortDef = new CShortDefinition();
			shortDef->push_back(new CMetaIdentifier(helperIdentifier2));

			AddDefinition(new CShortDefinition());
			AddDefinition(shortDef);
		}
		else if (option == GroupRepetition)
		{
			// H2 = H2, H1 | [empty] ?
			CShortDefinition* shortDef = new CShortDefinition();
			shortDef->push_back(new CMetaIdentifier(helperIdentifier2));
			shortDef->push_back(new CMetaIdentifier(helperIdentifier));
			AddDefinition(new CShortDefinition());
			AddDefinition(shortDef);
		}
	}
}