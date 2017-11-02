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
	CSyntaxRule(std::istream& is);
	virtual ~CSyntaxRule();
	const CMetaIdentifier& GetIdentifier() const;

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream& os) const override;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;

};
std::ostream& operator<<(std::ostream& os, const CSyntaxRule& rule);
