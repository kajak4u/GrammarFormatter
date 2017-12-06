#pragma once

#include <iostream>
#include "Enums.h"

void ExtractLinePos(std::istream& is, int& line, int& pos, std::string& lineContent);
void skipWhiteChars(std::istream& is);
Symbol GetSymbol(std::istream& is, int& group, bool alterStream);
Symbol GetSymbol(std::istream& is, bool alterStream);

template<typename _T> auto CompareObjects()
{
	using Type = bool(const _T*, const _T*);
	return [](const _T* a, const _T* b) {return *a < *b; };
}

class MyException
{
public:
	std::string message;
	int retCode;
	MyException(const std::string& message, int retCode)
		: message(message), retCode(retCode)
	{}
};