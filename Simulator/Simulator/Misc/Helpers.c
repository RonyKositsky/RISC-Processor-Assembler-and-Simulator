#include "Helpers.h"
#include <stdlib.h>
#include <string.h>

int GetDecimalFromHex(char* hexValue)
{
	return (int)strtol(hexValue, NULL, 16);
}

int GetDecimalFromHex2Comp(char* hexValue)
{
	if (hexValue[0] == '0')
		return GetDecimalFromHex(hexValue);

	return ((GetDecimalFromHex(hexValue) & 0xFFFFF) << 12) >> 12;
}

void RemoveLastChar(char* str) {
	str[strlen(str) - 1] = '\0';
}
