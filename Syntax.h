#pragma once

#include "Readable.h"
#include <vector>
#include "Readable.h"
#include "GrammarObject.h"

class CSyntaxRule;

class CSyntax :	public std::vector<CSyntaxRule*>, public IReadable, public CGrammarObject
{
public:
	CSyntax();
	CSyntax(const CSyntax& other);
	CSyntax(CSyntax&&) = default;
	virtual ~CSyntax();

	std::istream & ReadFrom(std::istream & is) override;
	void WriteTo(std::ostream & os) const override;
	bool IsCorrect(std::string& errors) const;

	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
};

std::ostream& operator<<(std::ostream& os, const CSyntax& syntax);