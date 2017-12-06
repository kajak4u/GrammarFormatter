#include "Recognizer.h"
#include "main.h"

CRecognizer& CRecognizer::instance()
{
	static CRecognizer instance;
	return instance;
}

void CRecognizer::registerType(ISpawnable* obj, std::string seq)
{
	instance().hierarchy.add(seq, obj);
}

ISpawnable* CRecognizer::CreateFor(std::istream& is)
{
	skipWhiteChars(is);
	const ISpawnable* found = instance().hierarchy.Predict(is);
	return found == nullptr ? nullptr : found->spawn();
}