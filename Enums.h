#pragma once

#include "EnumSet.h"

enum Option { OptionNone, OptionRepetition, OptionOptional };
typedef CEnumSet<Option> Options;

enum Symbol
{
	SymbolUnknown,
	SymbolConcatenate,	// ,
	SymbolDefining,		// =
	SymbolSeparator,	// | \ !
	SymbolCommentEnd,	// *)
	SymbolCommentStart,	// (*
	SymbolGroupEnd,		// )
	SymbolGroupStart,	// (
	SymbolOptionEnd,	// ] /)
	SymbolOptionStart,	// [ (/
	SymbolRepeatEnd,	// } :)
	SymbolRepeatStart,	// { (:
	SymbolExcept,		// -
	SymbolQuote,		// ' "
	SymbolRepetition,	// *
	SymbolSpecial,		// ?
	SymbolTerminator	// ; .
};