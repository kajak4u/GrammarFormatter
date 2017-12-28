#pragma once

#include <iostream>
#include "Enums.h"

void ExtractLinePos(_STD istream& is, int& line, int& pos, _STD string& lineContent);
void skipWhiteChars(_STD istream& is);
Symbol GetSymbol(_STD istream& is, int& group, bool alterStream);
Symbol GetSymbol(_STD istream& is, bool alterStream);

template<typename _T> auto inline DereferenceOrNull(const typename _T::const_iterator iter, const _T& collection)
{
	return iter == collection.end() ? nullptr : *iter;
}

template<typename _T, typename _U> bool inline is(_U obj)
{
	return dynamic_cast<_T>(obj) != nullptr;
}

template<class _T> struct CompareObjects
{
	constexpr bool operator()(const _T* _Left, const _T* _Right) const
	{
		return _Left==nullptr ? _Right !=nullptr : _Right ==nullptr ? false : *_Left < *_Right;
	}
};

class MyException
{
public:
	_STD string message;
	int retCode;
	MyException(const _STD string& message, int retCode)
		: message(message), retCode(retCode)
	{}
};