#pragma once

#include "Readable.h"
#include <vector>
#include "GrammarObject.h"
#include "Spawnable.h"
#include "MySet.h"

class CTerm;
class CPrimary;
class CTerminal;

class IDefinition : public IReadable, public CGrammarObject, public ISpawnable
{
};

class CDefinition :	public std::vector<CTerm*>, public IDefinition
{
public:
	CDefinition();
	CDefinition(std::istream& is);
	CDefinition(const CDefinition& other);
	CDefinition(CDefinition&& other) = default;
	virtual ~CDefinition();
	std::istream& ReadFrom(std::istream& is) override;
	void WriteTo(std::ostream& os) const override;

	ISpawnable * spawn(bool copy = false) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;
};
std::ostream& operator<<(std::ostream& os, const CDefinition& def);

class CShortDefinition : public std::vector<CPrimary*>, public IDefinition
{
public:
	CShortDefinition();
	CShortDefinition(const CShortDefinition& other);
	CShortDefinition(CShortDefinition&&) = default;
	CShortDefinition(CDefinition* previous);
	virtual ~CShortDefinition();

	std::istream& ReadFrom(std::istream& is) override;
	void WriteTo(std::ostream& os) const override;

	ISpawnable * spawn(bool copy = false) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const override;
	void ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action) override;
};

MySet<CTerminal*> GetFirstFrom(std::vector<CPrimary*>::const_iterator begin, std::vector<CPrimary*>::const_iterator end);
