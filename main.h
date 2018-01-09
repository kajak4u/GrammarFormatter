#pragma once

#include <iostream>
#include "Enums.h"

//prints error location, provided that it occured in current [file] stream position
void PrintErrorPos(_STD istream& file, _STD ostream& output);
//extracts [line, char, lineContent] from current is position
void ExtractLinePos(_STD istream& is, int& line, int& pos, _STD string& lineContent);
//Skips whitespaces in given input stream. If [EBNFComment] is true, EBNF-style comments are also treated as whitespaces.
void SkipWhiteChars(_STD istream& is, bool EBNFComment=true);
//Takes EBNF symbol from input stream, returns its enumerate and characters expansion
std::pair<_STD string, Symbol> RecognizeSymbol(_STD istream& is);
//Recognizes EBNF symbol in given input stream. If [alterStream] is false, returns characters back to stream.
Symbol GetSymbol(_STD istream& is, bool alterStream);
//takes EBNF symbol from stream if it is equal to expected
bool TakeSymbolIf(_STD istream& is, Symbol expectedSymbol);

//returns null if given [iterator] is equal to given [collection]'s end, otherwise returns dereferenced [iterator]
template<typename _T> auto inline DereferenceOrNull(const typename _T::const_iterator iterator, const _T& collection)
{
	return iterator == collection.end() ? nullptr : *iterator;
}

//returns whether given [obj] can be cast to type [_T]
template<typename _T, typename _U> bool inline Is(_U obj)
{
	return dynamic_cast<_T>(obj) != nullptr;
}

//struct to comparing pointers using its object's compare methods
template<class _T> struct CompareObjects
{
	//if any pointer is null, the non-null is greater; otherwise use object's compare method
	constexpr bool operator()(const _T* _Left, const _T* _Right) const
	{
		return _Left==nullptr ? _Right !=nullptr : _Right ==nullptr ? false : *_Left < *_Right;
	}
};

//own exception class, allowing to assign exitcode to exception
class MyException
{
public:
#ifdef _DEBUG
	_STD string file;
	int line;
#endif
	_STD string message;
	int retCode;
#ifdef _DEBUG
#define MYEXCEPTION(message, retCode) MyException(message, retCode, __FILE__, __LINE__)
	MyException(const _STD string& message, int retCode, _STD string file, int line)
		: message(message), retCode(retCode), file(file), line(line)
	{}
#else
#define MYEXCEPTION(message, retCode) MyException(message, retCode)
	MyException(const _STD string& message, int retCode)
		: message(message), retCode(retCode)
	{}
#endif
};