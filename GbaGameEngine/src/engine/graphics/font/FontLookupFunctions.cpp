#include "FontLookupFunctions.h"

int StandardAsciiLookup(char c)
{
	return int(c);
}

int AsciiExclamationOffset(char c)
{
	return StandardAsciiLookup(c) - StandardAsciiLookup('!');
}
