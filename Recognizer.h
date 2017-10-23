#pragma once
#include "Tree.h"
#include "Spawnable.h"

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
	static void registerType(ISpawnable* obj, std::string seq);
	static ISpawnable* CreateFor(std::istream& is);
};