#pragma once

#include "Readable.h"
#include <vector>
#include "Enums.h"
#include "Spawnable.h"
#include "DefinitionList.h"

class CRuleGroup : public IReadable, public ISpawnable
{
	CDefinitionList definitionList;
	Options options;
public:
	virtual ~CRuleGroup();
	std::istream& ReadFrom(std::istream& is) override;
	ISpawnable* spawn() const;
	static void registerPrefixes();
};