#include "Helpers.h"
#include <stdlib.h>
#include <string.h>

void RemoveLastChar(char* str) {
	str[strlen(str) - 1] = '\0';
}

/*
 * hex value with '0x'
 */
int GetDecimalFromHex(char* hexValue)
{
	return (int)strtol(hexValue + 2, NULL, 16);
}

void GetHexValueOfConstant(uint num, char* hexVal, int numOfBytes) {
	const int base = 16;
	
	hexVal[numOfBytes] = '\0';
	int i = numOfBytes - 1;

	do {
		hexVal[i] = "0123456789ABCDEF"[num % base];
		i--;
		num /= base;
	} while (i >= 0);
}

/* Getting the decimal value from hex or decimal string*/
int GetDecimalValueFromString(char* str)
{
	if (strlen(str) > 2 && str[1] == 'x' || str[1] == 'X')
	{
		// we with hex values
		// Convert to int
		return (int)strtol(str + 2, NULL, 16);
	}

	// we with decimal value
	return atoi(str);
}

/* Check if one of registers is Immediate register*/
int HasImmediate(char* rd, char* rs, char* rt) {
	if (!strcmp(rd, "$imm,") || !strcmp(rs, "$imm,") || !strcmp(rt, "$imm,"))
		return 1;
	if (!strcmp(rd, "$imm") || !strcmp(rs, "$imm") || !strcmp(rt, "$imm"))
		return 1;

	return 0;
}
