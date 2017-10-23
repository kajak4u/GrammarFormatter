#include "main.h"
#include "Terminal.h"
#include "Group.h"
#include "MetaIdentifier.h"

int main(int argc, char* argv[])
{
	CTerminal::registerPrefixes();
	CGroup::registerPrefixes();
	CMetaIdentifier::registerPrefixes();
	return 0;
}

void skipWhiteChars(std::istream& is)
{
	char c;
	while (1)
		switch (c = is.get())
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			break;
		default:
			is.putback(c);
			return;
		}
}