#pragma once

enum Option { OptionNone, OptionRepetition, OptionOptional };

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

enum FormatEffect
{
	FormatNone,
	FormatTab,
	FormatSpace,
	FormatNoSpace,
	FormatNewLine,
	FormatIntend,
	FormatDedend
};