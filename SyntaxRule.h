#pragma once

#include "Readable.h"
#include "MetaIdentifier.h"
#include "DefinitionList.h"
#include <functional>
#include "GrammarObject.h"

class CSyntaxRule : public IReadable, public CGrammarObject
{
	CMetaIdentifier identifier;
	CDefinitionList definitionList;
public:
	CSyntaxRule();
	CSyntaxRule(const CMetaIdentifier& identifier);
	CSyntaxRule(std::istream& is);
	virtual ~CSyntaxRule();
	const CMetaIdentifier& GetIdentifier() const;
	const CDefinitionList& GetDefinitionList() const;

	void AddDefinition(CDefinition* definition);
	void AddCopyDefinition(const CDefinition* definition);

	std::istream & ReadFrom(std::istream & is) override;
	virtual void WriteTo(std::ostream& os) const override;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;

};
std::ostream& operator<<(std::ostream& os, const CSyntaxRule& rule);
