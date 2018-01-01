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
			AddCopyDefinition(item);
	}

	CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier & helperIdentifier, const CMetaIdentifier & helperIdentifier2, GroupType option)
		: CSyntaxRule(helperIdentifier)
	{
		if (option == GroupOptional)
		{
			//add definition: H2 = H1 | [empty]
			AddDefinition(new CShortDefinition());
			AddDefinition(new CShortDefinition
			{
				new CMetaIdentifier(helperIdentifier2)
			});
		}
		else if (option == GroupRepetition)
		{
			//add definition: H2 = H2, H1 | [empty];
			AddDefinition(new CShortDefinition());
			AddDefinition(new CShortDefinition
			{
				new CMetaIdentifier(helperIdentifier2),
				new CMetaIdentifier(helperIdentifier)
			});
		}
	}
}