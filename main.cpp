#include "main.h"

int main(int argc, char* argv[])
{
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