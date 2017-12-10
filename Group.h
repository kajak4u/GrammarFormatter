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
	Option options = OptionNone;
public:
	Option GetType() const;
	CGroup();
	CGroup(const CGroup& other) = default;
	CGroup(CGroup&& other) = default;
	CGroup(const CDefinitionList& list, Option type);
	CGroup(Option type);
	CGroup& operator=(CGroup&& other) = default;
	virtual ~CGroup();
	std::istream& ReadFrom(std::istream& is) override;
	const CDefinitionList& getDefinitionList() const;
	ISpawnable* spawn(bool copy = false) const override;
	static void registerPrefixes();
	void WriteTo(std::ostream & os) const override;
	bool Equals(const CPrimary * other) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;
};
std::ostream& operator<<(std::ostream& os, const CGroup& group);