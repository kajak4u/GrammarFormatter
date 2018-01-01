#include "Factor.h"
#include "Group.h"
#include "HelperSyntaxRule.h"
#include "ShortDefinition.h"
#include "Syntax.h"
#include "Terminal.h"
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
			SkipWhiteChars(is);
		} while (!is.eof());
		return is;
	}

	void CSyntax::WriteTo(ostream & os) const
	{
		for (CSyntaxRule* rule : *this)
			os << *rule << endl;
	}

	void CSyntax::CreateSets()
	{
		//create sets: FIRST(a) and FOLLOW(a) for each identifier a

		//FIRST first step - add terminals and empty symbol, add symbols to collection
		map<const CShortDefinition*, const CDefinedGrammarSymbol*> startingWithSymbol;
		CDefinedGrammarSymbol::ForEach([&startingWithSymbol](const CDefinedGrammarSymbol* symbol)
		{
			auto& defList = symbol->GetDefinitions();
			for (const CDefinition* def : defList)
			{
				const CShortDefinition* sdef = dynamic_cast<const CShortDefinition*>(def);
				if (sdef == nullptr)
					throw MYEXCEPTION("Only shortdefinitions allowed", 1);
				//if production is empty, FIRST should contain empty symbol
				if (sdef->empty())
					symbol->First() += nullptr;
				else
				{
					auto& primary = *sdef->begin();
					//if production begins with terminal, add it to FIRST set
					if (is<CTerminal*>(primary))
						symbol->First() += dynamic_cast<CTerminal*>(primary);
					//if production begins with symbol, it should be process in a loop
					else if (is<CDefinedGrammarSymbol*>(primary))
						startingWithSymbol.insert({ sdef, dynamic_cast<CDefinedGrammarSymbol*>(symbol->spawn(true)) });
					else
						throw MYEXCEPTION("Unexpected null pointer in definition", 1);
				}
			}
		});

		//FIRST second step - add terminals from productions until there are no changes
		for (bool changed = true; !(changed = !changed);)
			for (auto& keyVal : startingWithSymbol)
				changed = keyVal.second->TryAddFirstFrom(keyVal.first) || changed;
		//clear memory
		for (auto& keyVal : startingWithSymbol)
			delete keyVal.second;

		//FOLLOW first step - add terminals from productions, add iterators to collection
		set<pair<const CDefinedGrammarSymbol*, const CDefinedGrammarSymbol*> > followedPairs;
		CDefinedGrammarSymbol::ForEach([&followedPairs](const CDefinedGrammarSymbol* symbol)
		{
			auto& defList = symbol->GetDefinitions();
			for (const CDefinition* def : defList)
			{
				const CShortDefinition* sdef = dynamic_cast<const CShortDefinition*>(def);
				for (auto& iter = sdef->begin(), nextIter = iter; iter != sdef->end(); ++iter)
				{
					++nextIter;
					CDefinedGrammarSymbol* currentId = dynamic_cast<CDefinedGrammarSymbol*>(*iter);
					if (!currentId)
						continue; //we look for x,[nonterminal],y
					//if currentId is followed by terminal, add it to FOLLOW set
					CTerminal* nextTerminal = dynamic_cast<CTerminal*>(DereferenceOrNull(nextIter, *sdef));
					if (nextTerminal)
						currentId->Follow() += nextTerminal;
					else // (*nextIter == sdef->end() || dynamic_cast<CMetaIdentifier*>(*nextIter))
					{
						MySet<CTerminal*, CompareObjects<CTerminal>> firstFromFollowing = GetFirstFrom(nextIter, sdef->end());
						//if FIRST from [nextIter..sdef->end()] doesn't contain empty symbol, add it to FOLLOW set
						if (!firstFromFollowing.Contains(nullptr))
							currentId->Follow() += firstFromFollowing;
						else
						{
							//add all symbols except empty and add production for processing later
							currentId->Follow() += (firstFromFollowing - nullptr);
							followedPairs.insert({ currentId, dynamic_cast<CDefinedGrammarSymbol*>(symbol->spawn(true)) });
						}
					}
				}
			}
		});

		//FOLLOW second step - add terminals from productions until there are no changes
		for (bool changed = true; !(changed = !changed);)
			for (auto& elem : followedPairs)
				if (!elem.second->Follow().IsSubsetOf(elem.first->Follow()))
				{
					changed = true;
					elem.first->Follow() += elem.second->Follow();
				}
		//clear memory
		for (auto& keyVal : followedPairs)
			delete keyVal.second;
	}

	void CSyntax::Simplify()
	{
		vector<CFactor*> groupsToReplace;
		//iterate over each factors to find all groups
		ForEach(
			[](const CGrammarObject* symbol)
			{
				const CFactor* factor = dynamic_cast<const CFactor*>(symbol);
				return factor != nullptr && is<const CGroup*>(factor->GetPrimary());
			},
				[&groupsToReplace](CGrammarObject* symbol) {
				groupsToReplace.push_back(dynamic_cast<CFactor*>(symbol));
			}
		);
		//iterate backward, because a group can contain another group and internal group, that should be processed first, is always later in the array
		for (auto iter = groupsToReplace.rbegin(); iter != groupsToReplace.rend(); ++iter)
		{
			CFactor* factor = *iter;
			const CGroup* group = dynamic_cast<const CGroup*>(factor->GetPrimary());
#ifdef DEBUG_SIMPLIFY
			cerr << "Replace:" << endl << *group << endl << " For: " << endl;
#endif
			//replace group to one or many helper rules
			//first rule is always group's definition list
			CMetaIdentifier identifier("HS#" + to_string(++helperRulesCounter));
			const CDefinitionList& defList = group->getDefinitionList();
			CHelperSyntaxRule* helperRule = new CHelperSyntaxRule(identifier, defList);
#ifdef DEBUG_SIMPLIFY
			cerr << *helperRule << endl;
#endif
			this->push_back(helperRule);
			//if group is default, everything is now fine
			if (group->GetType() == GroupDefault)
			{
				factor->SetPrimary(&identifier);
				identifier.MarkAsUsed();
			}
			else
			{
				//if group was optional or repetition, then an additional rule should be added to keep it
				CMetaIdentifier identifier2("HS#" + to_string(++helperRulesCounter));
				CHelperSyntaxRule* helperRule2 = new CHelperSyntaxRule(identifier2, identifier, group->GetType());
#ifdef DEBUG_SIMPLIFY
				cerr << " and: " << endl << *helperRule2 << endl;
#endif
				identifier.MarkAsUsed();
				this->push_back(helperRule2);
				factor->SetPrimary(&identifier2);
				identifier2.MarkAsUsed();
			}
		}

		//once all groups have been removed, remaining rules may be safely simplified to primary objects' sequences
		for (CSyntaxRule* rule : *this)
		{
#ifdef DEBUG_SIMPLIFY
			cerr << "Simplify: " << endl << *rule << endl;
			rule->Simplify();
			cerr << *rule << endl;
#else
			rule->Simplify();
#endif
		}
	}

	bool CSyntax::IsCorrect(string & errors)
	{
		MySet<string> undefined, unused;
		if (!CMetaIdentifier::GetWarnings(undefined, unused))
		{
			//the only unused symbol is syntax's start symbol
			CMetaIdentifier* currentStartSymbol = new CMetaIdentifier(*unused.begin());
			//new rule will be added: S# = [currentStartSymbol];
			CMetaIdentifier newStartSymbol = CMetaIdentifier("S#");
			//S# FOLLOW set contains unique terminal that will be added at the end of input file
			newStartSymbol.Follow() += CTerminal::Unique();
			CSyntaxRule* newRule = new CSyntaxRule(newStartSymbol);
			newRule->AddDefinition(new CShortDefinition
			{
				currentStartSymbol
			});
			newRule->Simplify();
			currentStartSymbol->MarkAsUsed();
			//add new rule to syntax
			push_back(newRule);
#ifdef DEBUG_PRINTMEM
			cerr << "Start symbol is " << *currentStartSymbol << endl;
#endif
			//change start symbol to S#
			startSymbol = newStartSymbol;
			return true;
		}
		else if (!undefined.empty())
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

	const CMetaIdentifier* CSyntax::GetStartSymbol() const
	{
		return &startSymbol;
	}

	void CSyntax::ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const
	{
		CGrammarObject::ForEach(condition, action);
		for (const CSyntaxRule* rule : *this)
			rule->ForEach(condition, action);
	}

	void CSyntax::ForEach(GrammarObjectPredicate condition, GrammarObjectAction action)
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