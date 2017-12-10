
#include "ShortDefinition.h"
#include "Syntax.h"
#include "SyntaxRule.h"
#include "main.h"
#include "Group.h"
#include "Factor.h"
#include "HelperSyntaxRule.h"
#include <map>

using namespace std;

namespace GrammarSymbols
{
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

	void CSyntax::WriteTo(ostream & os) const
	{
		for (CSyntaxRule* rule : *this)
			os << *rule << endl;
	}


	class CShortDefinitionIterator
	{
		using Iter = CShortDefinition::iterator;
		Iter _begin;
		Iter _end;
	public:
		Iter pos;

		CShortDefinitionIterator(CShortDefinition& def, Iter pos)
			: _begin(def.begin()), _end(def.end()), pos(pos)
		{}
		CShortDefinitionIterator(CShortDefinition& def)
			: CShortDefinitionIterator(def, def.begin())
		{}
		const Iter begin() const { return _begin; }
		const Iter end() const { return _end; }
	};

	void CSyntax::CreateSets()
	{
		//create sets: FIRST(a) and FOLLOW(a) for each identifier a

		//FIRST first step - add terminals and empty symbol, add symbols to collection
		map<const CShortDefinition*, const CMetaIdentifier*> startingWithSymbol;
		for (CSyntaxRule* rule : *this)
		{
			auto& identifier = rule->GetIdentifier();
			auto& defList = rule->GetDefinitionList();
			for (IDefinition* def : defList)
			{
				if (def == nullptr)
					identifier.First() += nullptr;
				else
				{
					CShortDefinition* sdef = dynamic_cast<CShortDefinition*>(def);
					if (sdef == nullptr)
						throw MyException("Only shortdefinitions allowed\n" __FILE__, __LINE__);
					if (sdef->size() == 0)
						throw MyException("Unexpected empty definition\n" __FILE__, __LINE__);
					CPrimary* primary = *sdef->begin();
					if (CTerminal* terminal = dynamic_cast<CTerminal*>(primary))
						identifier.First() += terminal;
					else if (dynamic_cast<CMetaIdentifier*>(primary))
						startingWithSymbol.insert({ sdef, &identifier });
				}
			}
		}
		//FIRST second step - add terminals from productions until there are no changes
		for (bool changed = true; !(changed = !changed);)
			for (auto& keyVal : startingWithSymbol)
				changed = keyVal.second->TryAddFirstFrom(keyVal.first) || changed;

		//FOLLOW first step - add terminals from productions, add iterators to collection
		set<pair<const CMetaIdentifier*, const CMetaIdentifier*> > followedPairs;
		for (CSyntaxRule* rule : *this)
		{
			auto& ruleIdentifier = rule->GetIdentifier();
			auto& defList = rule->GetDefinitionList();
			for (IDefinition* def : defList)
			{
				CShortDefinition* sdef = dynamic_cast<CShortDefinition*>(def);
				if (sdef == nullptr)
					continue;
				for (auto& iter = sdef->begin(), nextIter = iter; iter != sdef->end(); ++iter)
				{
					++nextIter;
					CMetaIdentifier* currentId = dynamic_cast<CMetaIdentifier*>(*iter);
					if (!currentId)
						continue; //we look for x,[nonterminal],y
					CTerminal* nextTerminal = dynamic_cast<CTerminal*>(DereferenceOrNull(nextIter, *sdef));
					if (nextTerminal)
						currentId->Follow() += nextTerminal;
					else // (*nextIter == sdef->end() || dynamic_cast<CMetaIdentifier*>(*nextIter))
					{
						MySet<CTerminal*> firstFromFollowing = GetFirstFrom(nextIter, sdef->end());
						if (!firstFromFollowing.Contains(nullptr))
							currentId->Follow() += firstFromFollowing;
						else
						{
							currentId->Follow() += (firstFromFollowing - nullptr);
							followedPairs.insert({ currentId, &ruleIdentifier });
						}
					}
				}
			}
		}

		//FOLLOW second step - add terminals from productions until there are no changes
		for (bool changed = true; !(changed = !changed);)
			for (auto& elem : followedPairs)
				if (!elem.second->Follow().IsSubsetOf(elem.first->Follow()))
				{
					changed = true;
					elem.first->Follow() += elem.second->Follow();
				}
	}

	void CSyntax::Simplify()
	{
		vector<CFactor*> groupsToReplace;
		ForEach(
			[](const CGrammarObject* symbol)
		{
			const CFactor* factor = dynamic_cast<const CFactor*>(symbol);
			return factor != nullptr && dynamic_cast<const CGroup*>(factor->GetPrimary()) != nullptr;
		},
			[&groupsToReplace](CGrammarObject* symbol) {
			groupsToReplace.push_back(dynamic_cast<CFactor*>(symbol));
		}
		);
		//idziemy od konca - dlatego, zeby w przypadku zawierania najpierw podmienic grupe wewnetrzna
		for (auto iter = groupsToReplace.rbegin(); iter != groupsToReplace.rend(); ++iter)
		{
			CFactor* factor = *iter;
			const CGroup* group = dynamic_cast<const CGroup*>(factor->GetPrimary());
			cerr << "Replace:" << endl << *group << endl << " For: " << endl;
			CMetaIdentifier identifier("HS#" + to_string(++helperRulesCounter));
			const CDefinitionList& defList = group->getDefinitionList();
			CHelperSyntaxRule* helperRule = new CHelperSyntaxRule(identifier, defList);
			cerr << *helperRule << endl;
			this->push_back(helperRule);
			if (group->GetType() == OptionNone)
			{
				factor->SetPrimary(&identifier);
				identifier.MarkAsUsed();
			}
			else
			{
				CMetaIdentifier identifier2("HS#" + to_string(++helperRulesCounter));
				CHelperSyntaxRule* helperRule2 = new CHelperSyntaxRule(identifier2, identifier, group->GetType());
				cerr << " and: " << endl << *helperRule2 << endl;
				identifier.MarkAsUsed();
				this->push_back(helperRule2);
				factor->SetPrimary(&identifier2);
				identifier2.MarkAsUsed();
			}
		}

		for (CSyntaxRule* rule : *this)
		{
			cerr << "Simplify: " << endl << *rule << endl;
			rule->Simplify();
			cerr << *rule << endl;
		}
	}

	bool CSyntax::IsCorrect(string & errors)
	{
		MySet<string> undefined, unused;
		if (!CMetaIdentifier::GetWarnings(undefined, unused))
		{
			CMetaIdentifier* currentStartSymbol = new CMetaIdentifier(*unused.begin());
			CShortDefinition* def = new CShortDefinition();
			def->push_back(currentStartSymbol);
			CMetaIdentifier newStartSymbol = CMetaIdentifier("S#");
			CSyntaxRule* newRule = new CSyntaxRule(newStartSymbol);
			newRule->AddDefinition(def);
			newRule->Simplify();
			currentStartSymbol->MarkAsUsed();
			push_back(newRule);
			cerr << "Start symbol is " << *currentStartSymbol << endl;
			startSymbol = newStartSymbol;
			return true;
		}
		if (!undefined.empty())
		{
			for (auto& name : undefined)
				errors += string(errors == "" ? "" : "\n") + "Error: symbol " + name + " was not declared.";
		}
		else if (unused.empty())
		{
			errors += string(errors == "" ? "" : "\n") + "Error - cannot deduce start symbol - no unused symbols.\n"
				+ "\tNote: there should be exactly one symbol defined but not used.";
		}
		else
		{
			for (auto& name : unused)
				errors += string(errors == "" ? "" : "\n") + "Error: symbol " + name + " was never used.";
			errors += "\n\tNote: there should be exactly one symbol defined but not used.";
		}
		return false;
	}

	set<const CMetaIdentifier*, CMetaIdentifier::ComparePointers> CSyntax::GetAllIdentifiers() const
	{
		set <const CMetaIdentifier*, CMetaIdentifier::ComparePointers> identifiers(CompareObjects<CMetaIdentifier>());
		for (const CSyntaxRule* rule : *this)
			identifiers.insert(&rule->GetIdentifier());
		return identifiers;
	}

	set<const CTerminal*, CTerminal::ComparePointers> CSyntax::GetAllTerminals() const
	{
		set <const CTerminal*, CTerminal::ComparePointers> terminals(CompareObjects<CTerminal>());
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

	const CMetaIdentifier CSyntax::GetStartSymbol() const
	{
		return startSymbol;
	}

	void CSyntax::ForEach(function<bool(const CGrammarObject*)> condition, function<void(const CGrammarObject*)> action) const
	{
		CGrammarObject::ForEach(condition, action);
		for (const CSyntaxRule* rule : *this)
			rule->ForEach(condition, action);
	}

	void CSyntax::ForEach(function<bool(const CGrammarObject*)> condition, function<void(CGrammarObject*)> action)
	{
		CGrammarObject::ForEach(condition, action);
		for (CSyntaxRule* rule : *this)
			rule->ForEach(condition, action);
	}

	ostream & operator<<(ostream & os, const CSyntax & syntax)
	{
		syntax.WriteTo(os);
		return os;
	}
}