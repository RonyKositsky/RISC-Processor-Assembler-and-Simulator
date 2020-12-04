#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGTH 500
#define MEMORY_ADDRESS_NUM 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;
typedef struct {
	char* LabelName;
	uint LabelLine;
}Label;

Label* LabelsLocations = NULL;
uint labelsCounter = 0;

char* memoryAddress[MEMORY_ADDRESS_NUM];

void RemoveLastChar(char* str) {
	str[strlen(str) - 1] = '\0';
}

/*
 * hex value should be in length of numOfBytes + 1.
 */
void GetHexValueOfConstant(char* immValue, char* hexVal, int numOfBytes) {
	const int base = 16;
	uint num = atoi(immValue);

	hexVal[numOfBytes] = '\0';
	int i = numOfBytes - 1;

	do {
		hexVal[i] = "0123456789ABCDEF"[num % base];
		i--;
		num /= base;
	} while (i >= 0);
}

/*
 * hex value with '0x'
 */
int GetDecimalFromHex(char* hexValue)
{
	return (int)strtol(hexValue+2, NULL, 16);
}

void InitMemoryAddress()
{
	for (int i = 0; i < MEMORY_ADDRESS_NUM; i++)
	{
		memoryAddress[i] = NULL;
	}
}

int HasImmediate(char* rd, char* rs, char* rt) {
	if (!strcmp(rd, "$imm,") || !strcmp(rs, "$imm,") || !strcmp(rt, "$imm,"))
		return 1;
	if (!strcmp(rd, "$imm") || !strcmp(rs, "$imm") || !strcmp(rt, "$imm"))
		return 1;

	return 0;
}

int InsideLabelList(char* potentialLabel) {
	for (int i = 0; i < labelsCounter; i++) {
		if (!strcmp(potentialLabel, (*(LabelsLocations + i)).LabelName)) {
			return 1;
		}
	}

	return 0;
}

int GetLabelLine(char* label) {
	for (int i = 0; i < (int) labelsCounter; i++) {
		if (!strcmp(label, (*(LabelsLocations + i)).LabelName)) {
			return (int)LabelsLocations[i].LabelLine;
		}
	}
}

void ExtractLabels(FILE* asmFile) {
	char line[MAX_LINE_LENGTH];
	int lineCounter = 0;

	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL)
	{
		char* opcode = (char*)calloc(20, sizeof(char));
		char rd[20], rs[20], rt[20], immVal[20];

		// Search for command line
		if (!sscanf(line, "%s %s %s %s %s", opcode, rd, rs, rt, immVal)) {
			if (opcode[strlen(opcode) - 1] == ':') { // This is label
				RemoveLastChar(opcode);
				Label labelStruct = { &(*opcode), lineCounter };
				if (LabelsLocations == NULL) {
					LabelsLocations = (Label*)calloc(1, sizeof(Label));
					*LabelsLocations = labelStruct;
				}
				else {
					LabelsLocations = realloc(LabelsLocations, (labelsCounter + 1) * sizeof(Label));
					*(LabelsLocations + labelsCounter) = labelStruct;
				}

				labelsCounter++;
			}
			else if (HasImmediate(rd, rs, rt))
				lineCounter += 2;
			else
				lineCounter++;
		}
		// TODO: Free allocations
	}
}

void ExtractCommands(FILE* asmFile, FILE* imemFile) {
	char line[MAX_LINE_LENGTH];
	int lineCounter = 0;
	rewind(asmFile);
	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL) {
		char opcode[20], rd[20], rs[20], rt[20], immVal[20];

		// Search for command line
		// TODO: Test what happen when sscanf not equal to sequence  
		if (sscanf(line, "%s %s %s %s %s", opcode, rd, rs, rt, immVal)) {
			if (opcode[strlen(opcode) - 1] == ':') // This is label
				continue;

			if (!strcmp(opcode, ".word"))
			{
				int address;
				char val[9];
				val[8] = '\0';
				// Address
				if (strlen(rd) > 2 && rd[1] == 'x' || rd[1] == 'X')
				{
					// we with hex values
					// Convert to int
					address = GetDecimalFromHex(rd);
				}
				else
				{
					// we with decimal value
					address = atoi(rd);
				}

				// Value
				if (strlen(rs) > 2 && rs[1] == 'x' || rs[1] == 'X')
				{
					// we with hex values
					const int len = (int) strlen(rs);
					int j = 0;
					for (int i = 7; i >= 0; i--)
					{
						if (j < len - 2)
							val[i] = rs[len - 1 - j];
						else
							val[i] = '0';

						j++;
					}
				}
				else
				{
					// we with decimal value
					GetHexValueOfConstant(rs, val, 8);
				}

				memoryAddress[address] = (char*)calloc(9, sizeof(char));
				strcpy(memoryAddress[address], val);
				continue;
			}

			// Removing ','
			RemoveLastChar(rd); RemoveLastChar(rs); RemoveLastChar(rt);

			// Getting Opcode number.
			char* opcodeNumber = GetOpcodeNumber(opcode);
			fputs(opcodeNumber, imemFile);

			// Getting registers numbers
			char* rdNumber = GetRegisterNumber(rd);
			fputs(rdNumber, imemFile);
			char* rsNumber = GetRegisterNumber(rs);
			fputs(rsNumber, imemFile);
			char* rtNumber = GetRegisterNumber(rt);
			fputs(rtNumber, imemFile);
			fputc('\n', imemFile);

			if (InsideLabelList(immVal)) {
				int lineLabel = GetLabelLine(immVal);
				char line_arr[6], hexValue[6];
				sprintf(line_arr, "%d", lineLabel); //Getting line number from the file.
				GetHexValueOfConstant(line_arr, hexValue, 5); //Getting line number as hex.
				fputs(hexValue, imemFile);
				fputc('\n', imemFile);
			}
			else {
				// if there is $imm then convert immVal to Hex
				if (HasImmediate(rd, rs, rt)) {
					char hexValue[6];
					GetHexValueOfConstant(immVal, hexValue, 5);
					fputs(hexValue, imemFile);
					fputc('\n', imemFile);
				}
			}
		}
	}
}

void ExtractMemoryFile(FILE* dmemFile)
{
	for (int i = 0; i < MEMORY_ADDRESS_NUM; i++)
	{
		char* addressValue = memoryAddress[i];
		if (addressValue == NULL)
			fputs("00000000", dmemFile);
		else
			fputs(addressValue, dmemFile);

		fputc('\n', dmemFile);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 4)
		return 1;
	
	FILE* asmFile = fopen(argv[1], "r");
	FILE* imemFile = fopen(argv[2], "w");
	FILE* dmemFile = fopen(argv[3], "w");
	if (asmFile == NULL || imemFile == NULL || dmemFile == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	InitMemoryAddress();
	
	ExtractLabels(asmFile);
	ExtractCommands(asmFile, imemFile);
	ExtractMemoryFile(dmemFile);

	// TODO: Free all allocations
	
	fclose(asmFile);
	fclose(imemFile);
	fclose(dmemFile);
	return 0;
}
