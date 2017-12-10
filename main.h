#pragma once

#include <iostream>
#include "Enums.h"

void ExtractLinePos(_STD istream& is, int& line, int& pos, _STD string& lineContent);
void skipWhiteChars(_STD istream& is);
Symbol GetSymbol(_STD istream& is, int& group, bool alterStream);
Symbol GetSymbol(_STD istream& is, bool alterStream);

template<typename _T> auto DereferenceOrNull(const typename _T::const_iterator iter, const _T& collection)
{
	return iter == collection.end() ? nullptr : *iter;
}

template<typename _T> auto CompareObjects()
{
	using Type = bool(const _T*, const _T*);
	return [](const _T* a, const _T* b) {return *a < *b; };
}

class MyException
{
public:
	_STD string message;
	int retCode;
	MyException(const _STD string& message, int retCode)
		: message(message), retCode(retCode)
	{}
};