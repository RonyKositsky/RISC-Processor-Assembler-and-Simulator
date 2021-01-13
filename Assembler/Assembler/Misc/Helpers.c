#include "Helpers.h"
#include <stdlib.h>
#include <string.h>

void RemoveLastComma(char* str) {
	if (str[strlen(str) - 1] == ',')
		str[strlen(str) - 1] = '\0';
}

void RemoveLastChar (char* str) {
	str[strlen(str) - 1] = '\0';
}

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

int GetDecimalValueFromString(char* str)
{
	// If has "0x"
	if (strlen(str) > 2 && str[1] == 'x' || str[1] == 'X')
	{
		// Convert hex value to int (removing "0x")
		return (int)strtol(str + 2, NULL, 16);
	}

	// Convert string to decimal value
	return atoi(str);
}

int HasImmediate(char* rd, char* rs, char* rt) {
	// Check if one of registers is Immediate register

	if (!strcmp(rd, "$imm,") || !strcmp(rs, "$imm,") || !strcmp(rt, "$imm,"))
		return 1;
	if (!strcmp(rd, "$imm") || !strcmp(rs, "$imm") || !strcmp(rt, "$imm"))
		return 1;

	return 0;
}
