#include "MetaIdentifierManager.h"
#include <iostream>
#include "Terminal.h"

std::ostream& operator<<(std::ostream& os, const CMetaIdentifierManagerItem& item)
{
	os << "First: {";
	for (auto& elem : item.first)
		if (elem)
			os << *elem << " ";
		else
			os << "[] ";
	os << "}\tFollow: {";
	for (auto& elem : item.follow)
		if (elem)
			os << *elem << " ";
		else
			os << "[] ";
	os << "};\t" << (item.used ? "" : "not ") << "used; " << (!item.definitions.empty() ? "" : "not ") << "defined";
	return os;
}
