#pragma once

#include <iostream>
#include "Enums.h"

void skipWhiteChars(std::istream& is);
Symbol GetSymbol(std::istream& is, int& group, bool alterStream);
Symbol GetSymbol(std::istream& is, bool alterStream);