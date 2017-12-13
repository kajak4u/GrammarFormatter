#pragma once
#include "Tree.h"
#include "Spawnable.h"

namespace GrammarSymbols
{
	class CRecognizer
	{
		CTree<ISpawnable> hierarchy;
		CRecognizer() {}
	public:
		CRecognizer(CRecognizer const&) = delete;
		CRecognizer(CRecognizer&&) = delete;
		CRecognizer& operator=(CRecognizer const&) = delete;
		CRecognizer& operator=(CRecognizer &&) = delete;

		static CRecognizer& instance();
		static void FreeMemory();
		static void registerType(ISpawnable* obj, _STD string seq);
		static ISpawnable* CreateFor(_STD istream& is);
	};
}