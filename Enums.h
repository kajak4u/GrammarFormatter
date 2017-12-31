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
	FormatSpace,	//enable spacing between symbols
	FormatNoSpace,	//disable spacing between symbols
	FormatNewLine,	//insert new line and intend
	FormatIntend,	//increase intend
	FormatDedend	//decrease intend
};