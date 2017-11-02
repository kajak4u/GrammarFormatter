#include "Syntax.h"
#include "SyntaxRule.h"
#include "main.h"

using namespace std;

CSyntax::CSyntax()
{
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

bool CSyntax::IsCorrect(std::string & errors) const
{
	string warnings;
	bool result = true;
	auto compare = [](const CMetaIdentifier* a, const CMetaIdentifier* b) {return a->GetName() < b->GetName(); };
	std::set <const CMetaIdentifier*, bool(*)(const CMetaIdentifier*, const CMetaIdentifier*)>
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

void CSyntax::ForEach(std::function<bool(const CGrammarObject*)> condition, std::function<void(const CGrammarObject*)> action) const
{
	CGrammarObject::ForEach(condition, action);
	for(const CSyntaxRule* rule : *this)
		rule->ForEach(condition, action);
}

std::ostream & operator<<(std::ostream & os, const CSyntax & syntax)
{
	syntax.WriteTo(os);
	return os;
}
