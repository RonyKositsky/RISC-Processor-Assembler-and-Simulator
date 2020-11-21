#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGTH 500

#include <stdio.h>
#include <string.h>

typedef unsigned int uint;
typedef struct {
	char* LableName;
	uint LabelLine;
}Label;

Label* LablesLocations = NULL;
uint labelsCounter = 0;

void RemoveLastChar(char* str) {
	str[strlen(str) - 1] = '\0';
}

void GetHexValueOfConstant(char* immValue, char* hexVal) {
	int base = 16;
	uint num = atoi(immValue);

	hexVal[5] = '\0';
	int i = 4;

	do {
		hexVal[i] = "0123456789ABCDEF"[num % base];
		i--;
		num /= base;
	} while (i >= 0);
}

int HasImmediate(char* rd, char* rs, char* rt) {
	if (!strcmp(rd, "$imm,") || !strcmp(rs, "$imm,") || !strcmp(rt, "$imm,"))
		return 1;
	if (!strcmp(rd, "$imm") || !strcmp(rs, "$imm") || !strcmp(rt, "$imm"))
		return 1;

	return 0;
}

void ExtractLabels(FILE* asmFile) {
	char line[MAX_LINE_LENGTH];
	int lineCounter = 0;

	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL)
	{
		char opcode[20], rd[20], rs[20], rt[20], immVal[20];

		// Search for command line
		if (sscanf(line, "%s %s %s %s %s", opcode, rd, rs, rt, immVal) != NULL) {
			if (opcode[strlen(opcode) - 1] == ':') { // This is label
				RemoveLastChar(opcode);
				Label labelStruct = { opcode, lineCounter };
				if (LablesLocations == NULL) {
					LablesLocations = (Label*)calloc(1, sizeof(Label));
					LablesLocations[0] = labelStruct;
				}
				else {
					LablesLocations = realloc(LablesLocations, (labelsCounter + 1) * sizeof(Label));
				}

				labelsCounter++;
			}
			else if (HasImmediate(rd, rs, rt))
				lineCounter += 2;
			else
				lineCounter++;
		}
	}
}

void ExtractCommands(FILE* asmFile, FILE* imemFile) {
	char line[MAX_LINE_LENGTH];
	int lineCounter = 0;

	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL) {
		char opcode[20], rd[20], rs[20], rt[20], immVal[20];

		// Search for command line
		if (sscanf(line, "%s %s %s %s %s", opcode, rd, rs, rt, immVal) != NULL) {
			if (opcode[strlen(opcode) - 1] == ':') // This is label
				continue;

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

			// if there is $imm then convert immVal to Hex
			if (HasImmediate(rd, rs, rt)) {
				char hexValue[6];
				GetHexValueOfConstant(immVal, &hexValue);
				fputs(hexValue, imemFile);
				fputc('\n', imemFile);
			}
		}
	}
}


int main(int argc, char* argv[]) {
	if (argc != 4)
		return 1;
	
	FILE* asmFile = fopen(argv[1], "r");
	FILE* imemFile = fopen(argv[2], "w");
	if (asmFile == NULL || imemFile == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	//ExtractLabels(asmFile);
	ExtractCommands(asmFile, imemFile);
	
	fclose(asmFile);
	fclose(imemFile);
}