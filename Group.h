#pragma once

#include "Readable.h"
#include <vector>
#include "Enums.h"
#include "Spawnable.h"
#include "DefinitionList.h"
#include "Primary.h"

class CGroup : public CPrimary
{
	CDefinitionList definitionList;
	Option options;
public:
	virtual ~CGroup();
	std::istream& ReadFrom(std::istream& is) override;
	ISpawnable* spawn() const override;
	static void registerPrefixes();
	void WriteTo(std::ostream & os) const override;
};
std::ostream& operator<<(std::ostream& os, const CGroup& group);