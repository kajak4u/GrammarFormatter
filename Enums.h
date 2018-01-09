#pragma once

//CGroup type
enum GroupType
{
	GroupDefault,			// (...)
	GroupRepetition,	// {...}
	GroupOptional		// [...]
};

//EBNF grammar symbols
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

//format effects applied via special symbols ?...?
enum FormatEffect
{
	FormatNone,		//no format
	FormatTab,		//insert tab character
	FormatNoSpace,	//do not insert space when printing
	FormatWhitespaceEnable,	//allow whitespaces between symbols
	FormatWhitespaceDisable,//forbid whitespaces between symbols
	FormatNewLine,	//insert new line and intend
	FormatIntend,	//increase intend
	FormatDedend	//decrease intend
};