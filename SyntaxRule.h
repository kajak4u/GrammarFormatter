#pragma once

#include "Readable.h"
#include "MetaIdentifier.h"
#include "DefinitionList.h"

class CSyntaxRule : public IReadable
{
	CMetaIdentifier identifier;
	CDefinitionList definitionList;
public:
	CSyntaxRule();
	CSyntaxRule(std::istream& is);
	virtual ~CSyntaxRule();

	std::istream & ReadFrom(std::istream & is) override;
};

