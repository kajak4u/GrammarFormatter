#include "Recognizer.h"

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
	const ISpawnable* found = instance().hierarchy.get(is);
	return found == nullptr ? nullptr : found->spawn();
}