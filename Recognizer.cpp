#include "main.h"
#include "Recognizer.h"

namespace GrammarSymbols
{
	CRecognizer& CRecognizer::Instance()
	{
		static CRecognizer instance;
		return instance;
	}

	void CRecognizer::FreeMemory()
	{
		Instance().hierarchy.Clear();
	}

	void CRecognizer::RegisterType(ISpawnable* obj, _STD string seq)
	{
		Instance().hierarchy.Add(seq, obj);
	}

	ISpawnable* CRecognizer::CreateFor(_STD istream& is)
	{
		SkipWhiteChars(is);
		const ISpawnable* found = Instance().hierarchy.Predict(is);
		//if found something, create its empty clone, return null otherwise
		return found == nullptr ? nullptr : found->Spawn();
	}
}