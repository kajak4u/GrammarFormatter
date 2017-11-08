#pragma once

#include <iostream>
#include "Enums.h"

void ExtractLinePos(std::istream& is, int& line, int& pos, std::string& lineContent);
void skipWhiteChars(std::istream& is);
Symbol GetSymbol(std::istream& is, int& group, bool alterStream);
Symbol GetSymbol(std::istream& is, bool alterStream);


class MyException
{
public:
	std::string message;
	int retCode;
	MyException(const std::string& message, int retCode)
		: message(message), retCode(retCode)
	{}
};