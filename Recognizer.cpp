#include "Recognizer.h"
#include "main.h"

namespace GrammarSymbols
{
	CRecognizer& CRecognizer::instance()
	{
		static CRecognizer instance;
		return instance;
	}

	void CRecognizer::FreeMemory()
	{
		instance().hierarchy.clear();
	}

	void CRecognizer::registerType(ISpawnable* obj, _STD string seq)
	{
		instance().hierarchy.add(seq, obj);
	}

	ISpawnable* CRecognizer::CreateFor(_STD istream& is)
	{
		SkipWhiteChars(is);
		const ISpawnable* found = instance().hierarchy.Predict(is);
		//if found something, create its empty clone, return null otherwise
		return found == nullptr ? nullptr : found->spawn();
	}
}