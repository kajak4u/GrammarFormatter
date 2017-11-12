#pragma once

#include "Readable.h"
#include <vector>
#include <set>
#include "Readable.h"
#include "GrammarObject.h"
#include "Terminal.h"
#include "MetaIdentifier.h"

class CSyntaxRule;

class CSyntax :	public std::vector<CSyntaxRule*>, public IReadable, public CGrammarObject
{
	int helperRulesCounter = 0;
public:
	CSyntax();
	CSyntax(const CSyntax& other);
	CSyntax(CSyntax&&) = default;
	virtual ~CSyntax();

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream & os) const override;
	void PrepareForParsing();
	bool IsCorrect(std::string& errors) const;
	std::set<const CMetaIdentifier*, CMetaIdentifier::ComparePointers> GetAllIdentifiers() const;
	std::set<const CTerminal*, CTerminal::ComparePointers> GetAllTerminals() const;
	const CMetaIdentifier* GetStartSymbol() const;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;
};

std::ostream& operator<<(std::ostream& os, const CSyntax& syntax);