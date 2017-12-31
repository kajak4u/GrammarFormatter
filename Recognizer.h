#pragma once
#include "Tree.h"
#include "Spawnable.h"

namespace GrammarSymbols
{
	//class recognizing symbols from input stream, based on registered prefixes
	class CRecognizer
	{
		//hierarchy prefix tree
		CTree<ISpawnable> hierarchy;
		//private constructor
		CRecognizer() {}
	public:
		//copy constructor deleted
		CRecognizer(CRecognizer const&) = delete;
		//move constructor deleted
		CRecognizer(CRecognizer&&) = delete;
		//assign operator deleted
		CRecognizer& operator=(CRecognizer const&) = delete;
		//assign move operator deleted
		CRecognizer& operator=(CRecognizer &&) = delete;

		//returns reference to the only instance of class
		static CRecognizer& instance();
		//frees memory
		static void FreeMemory();
		//registers object sample with given prefix
		static void registerType(ISpawnable* obj, _STD string seq);
		//returns new object for corresponding input stream
		static ISpawnable* CreateFor(_STD istream& is);
	};
}