#pragma once

#include "Readable.h"
#include <vector>
#include "Enums.h"
#include "Spawnable.h"
#include "DefinitionList.h"
#include "Primary.h"

namespace GrammarSymbols
{

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
		_STD istream& ReadFrom(_STD istream& is) override;
		const CDefinitionList& getDefinitionList() const;
		ISpawnable* spawn(bool copy = false) const override;
		static void registerPrefixes();
		void WriteTo(_STD ostream & os) const override;
		bool Equals(const CPrimary * other) const override;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectConstAction action) const;
		void ForEach(GrammarObjectPredicate condition, GrammarObjectAction action) override;
	};
	_STD ostream& operator<<(_STD ostream& os, const CGroup& group);
}