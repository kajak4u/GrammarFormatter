#include "Syntax.h"
#include "SyntaxRule.h"
#include "main.h"
#include "Group.h"
#include "HelperSyntaxRule.h"

using namespace std;

CSyntax::CSyntax()
{
}

CSyntax::CSyntax(const CSyntax & other)
{
	this->reserve(other.size());
	for (CSyntaxRule* rule : other)
		this->push_back(new CSyntaxRule(*rule));
}


CSyntax::~CSyntax()
{
	for (CSyntaxRule*& rule : *this)
		delete rule;
}

istream & CSyntax::ReadFrom(istream & is)
{
	do
	{
		this->push_back(new CSyntaxRule(is));
		//cout << "Read rule: " << *this->back() << endl;
		skipWhiteChars(is);
	} while (!is.eof());
	return is;
}

void CSyntax::WriteTo(std::ostream & os) const
{
	for (CSyntaxRule* rule : *this)
		os << *rule << endl;
}

void CSyntax::PrepareForParsing()
{
	std::set<CGroup*> groupsToReplace;
	ForEach(
		[](const CGrammarObject* symbol)
		{
			const CGroup* group = dynamic_cast<const CGroup*>(symbol);
			return group != nullptr && group->GetType()==OptionRepetition;
		},
		[&groupsToReplace](CGrammarObject* symbol) {
			groupsToReplace.insert(dynamic_cast<CGroup*>(symbol));
		}
	);
	for (CGroup* group : groupsToReplace)
	{
		CMetaIdentifier identifier("helperRule" + std::to_string(++helperRulesCounter));
		CHelperSyntaxRule* helperRule = new CHelperSyntaxRule(identifier, group);
		this->push_back(helperRule);
		CGroup* newGroup = helperRule->CreateReplacement();
		*group = std::move(*newGroup);
		delete newGroup;
	}
}

bool CSyntax::IsCorrect(std::string & errors) const
{
	string warnings;
	bool result = true;
	auto compare = CompareObjects<CMetaIdentifier>();
	std::set <const CMetaIdentifier*, CMetaIdentifier::ComparePointers>
		defined(compare),
		used(compare);
	for (const CSyntaxRule* rule : *this)
	{
		defined.insert(&rule->GetIdentifier());
		rule->ForEach(
			[](const CGrammarObject* symbol)
			{
				return dynamic_cast<const CMetaIdentifier*>(symbol) != nullptr;
			},
			[&used](const CGrammarObject* symbol) {
				used.insert(dynamic_cast<const CMetaIdentifier*>(symbol));
			}
		);
	}
	auto defined_iter = defined.begin(), used_iter = used.begin();
	while(defined_iter != defined.end() && used_iter != used.end())
	{
		if (**defined_iter < **used_iter)
		{
			warnings += string(warnings == "" ? "" : "\n") + "Warning: symbol " + (*defined_iter)->GetName() + " was never used.";
			++defined_iter;
		}
		else if (**used_iter < **defined_iter)
		{
			result = false;
			errors += string(errors == "" ? "" : "\n") + "Error: symbol " + (*used_iter)->GetName() + " was not declared.";
			string usedIn;
			for (const CSyntaxRule* rule : *this)
				rule->ForEach(
					[&used_iter](const CGrammarObject* symbol)
					{
						auto obj = dynamic_cast<const CMetaIdentifier*>(symbol);
						return obj != nullptr && obj->GetName() == (*used_iter)->GetName();
					},
					[&usedIn, &rule](const CGrammarObject*)
					{
						usedIn += (usedIn==""?"":", ") + rule->GetIdentifier().GetName();
					}
				);
			errors += "\n\tReferenced in definition list of: " + usedIn;
			++used_iter;
		}
		else // (**defined_iter == **used_iter)
		{
			++defined_iter;
			++used_iter;
		}
	}
	if (errors == "")
		errors = warnings;
	return result;
}

std::set<const CMetaIdentifier*, CMetaIdentifier::ComparePointers> CSyntax::GetAllIdentifiers() const
{
	std::set <const CMetaIdentifier*, CMetaIdentifier::ComparePointers> identifiers(CompareObjects<CMetaIdentifier>());
	for (const CSyntaxRule* rule : *this)
		identifiers.insert(&rule->GetIdentifier());
	return identifiers;
}

std::set<const CTerminal*, CTerminal::ComparePointers> CSyntax::GetAllTerminals() const
{
	std::set <const CTerminal*, CTerminal::ComparePointers> terminals(CompareObjects<CTerminal>());
	for (const CSyntaxRule* rule : *this)
		rule->ForEach(
			[](const CGrammarObject* symbol)
			{
				return dynamic_cast<const CTerminal*>(symbol) != nullptr;
			},
			[&terminals](const CGrammarObject* symbol) {
				terminals.insert(dynamic_cast<const CTerminal*>(symbol));
			}
		);
	return terminals;
}

const CMetaIdentifier * CSyntax::GetStartSymbol() const
{
	//milczace zalozenie - pierwsza regula definiuje symbol startowy
	if (empty())
		return nullptr;
	return &(*begin())->GetIdentifier();
}

void CSyntax::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	for(const CSyntaxRule* rule : *this)
		rule->ForEach(condition, action);
}

void CSyntax::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(CGrammarObject*)> action)
{
	CGrammarObject::ForEach(condition, action);
	for (CSyntaxRule* rule : *this)
		rule->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CSyntax & syntax)
{
	syntax.WriteTo(os);
	return os;
}
