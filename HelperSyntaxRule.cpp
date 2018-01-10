#include "HelperSyntaxRule.h"
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

	CHelperSyntaxRule::CHelperSyntaxRule(const CMetaIdentifier & helperIdentifier, const CMetaIdentifier & helperIdentifier2)
		: CSyntaxRule(helperIdentifier)
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