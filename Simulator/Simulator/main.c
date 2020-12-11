#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "StaticDefinations.h"


/*
 * hex value without '0x'
 */
int GetDecimalFromHex(char* hexValue)
{
	return (int)strtol(hexValue, NULL, 16);
}

void MemoryInit(FILE* dmemInFile)
{
	char line[9];
	int lineCounter = 0;
	while (fgets(line, 9, dmemInFile) != NULL) {
		Memory[lineCounter] = GetDecimalFromHex(line);
		lineCounter++;
	}

	while (lineCounter < MEMORY_SIZE) {
		Memory[lineCounter] = 0;
		lineCounter++;
	}
}


int main(int argc, char* argv[]) {
	/*if (argc != 14)
		return 1;*/

	uint rd = 2, rs = 1 , rt = 0;
	RegisterMapping[1].RegisterValue = 10;
	OpcodeMapping[0].OperationFunc(rd, rs, rt);

	
	ProgramCounter = 0;
	FILE* imemInFile = fopen(argv[1], "r");
	FILE* dmemInFile = fopen(argv[2], "r");

	if (imemInFile == NULL || dmemInFile == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	MemoryInit(dmemInFile);


	fclose(imemInFile);
	fclose(dmemInFile);
}
