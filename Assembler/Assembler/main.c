#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGTH 500
#define MEMORY_ADDRESS_NUM 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Misc/Helpers.h"
#include "Misc/StaticDefinitions.h"

// Files
FILE* asmFile;
FILE* imemFile;
FILE* dmemFile;

// List of labels
Label* LabelsLocations = NULL;
uint labelsCounter = 0;

// Memory
char* memoryAddress[MEMORY_ADDRESS_NUM];

/* Init memory pointers to NULL */
void InitMemoryAddress()
{
	for (int i = 0; i < MEMORY_ADDRESS_NUM; i++)
	{
		memoryAddress[i] = NULL;
	}
}

int InsideLabelList(char* potentialLabel) {
	for (uint i = 0; i < labelsCounter; i++) {
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
	return -1;
}

void AddLabelToList(char* label, uint lineCounter)
{
	RemoveLastChar(label);
	Label labelStruct = { label, lineCounter };
	if (LabelsLocations == NULL) {
		LabelsLocations = (Label*)calloc(1, sizeof(Label));
		if (LabelsLocations != NULL)
			*LabelsLocations = labelStruct;
		else
		{
			printf("LabelsLocations allocation failed");
			exit(1);
		}
	}
	else {
		LabelsLocations = realloc(LabelsLocations, (labelsCounter + 1) * sizeof(Label));
		if (LabelsLocations != NULL)
			*(LabelsLocations + labelsCounter) = labelStruct;
		else
		{
			printf("LabelsLocations reallocation failed");
			exit(1);
		}
	}

	labelsCounter++;
}

void ExtractLabels() {
	char line[MAX_LINE_LENGTH];
	int lineCounter = 0;

	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL)
	{
		char* opcode = (char*)calloc(20, sizeof(char));
		char rd[20], rs[20], rt[20], immVal[20];
		rd[0] = '\0';

		// Search for command line
		if (sscanf(line, "%s%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]", opcode, rd, rs, rt, immVal)) 
		{
			if (opcode[0] == '\0' || opcode[0] == '#' || opcode[0]=='.')
				continue;
			// This is label
			if (opcode[strlen(opcode) - 1] == ':') { 
				AddLabelToList(opcode, lineCounter);

				if (rd[0] != '\0' && rd[0] != '#')
				{
					lineCounter += HasImmediate(rs, rt, immVal) ? 2 : 1;
				}
			}
			else
			{
				lineCounter += HasImmediate(rd, rs, rt) ? 2 : 1;
				free(opcode);
			}
		}	
	}
}

void HandleWordCommand(char* location, char* value)
{
	// Address
	int address = GetDecimalValueFromString(location);

	// Value
	int decValue = GetDecimalValueFromString(value);

	memoryAddress[address] = (char*)calloc(9, sizeof(char));
	if (memoryAddress[address] != NULL)
		sprintf(memoryAddress[address], "%08X", decValue);
	else
	{
		printf("memoryAddress allocation failed");
		exit(1);
	}
}

void HandleCommand(char* opcode, char* rd, char* rs, char* rt, char* immVal)
{
	if (!strcmp(opcode, ".word"))
	{
		HandleWordCommand(rd, rs);
		return;
	}

	// Removing ','
	RemoveLastComma(rd); RemoveLastComma(rs); RemoveLastComma(rt);
	
	// Writing Opcode number and registers
	fprintf(imemFile, "%s%s%s%s\n",
		GetOpcodeNumber(opcode), GetRegisterNumber(rd), GetRegisterNumber(rs), GetRegisterNumber(rt));
	
	if (InsideLabelList(immVal)) {
		int lineLabel = GetLabelLine(immVal);
		fprintf(imemFile, "%05X\n", lineLabel);
	}
	else {
		// if there is $imm then convert immVal to Hex
		if (HasImmediate(rd, rs, rt)) {
			int decValue = GetDecimalValueFromString(immVal);
			char hexValue[6];
			GetHexValueOfConstant(decValue, hexValue, 5);
			fprintf(imemFile, "%s\n", hexValue);
		}
	}
}

void ExtractCommands() {
	char line[MAX_LINE_LENGTH];
	rewind(asmFile);

	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL) {
		char opcode[20], rd[20], rs[20], rt[20], immVal[20], extraVal[20];
		opcode[0] = '\0';
		rd[0] = '\0';

		// Search for command line
		if (sscanf(line, "%s%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9[-]]%*[ ,\t]%[a-zA-Z$0-9[-]]", opcode, rd, rs, rt, immVal, extraVal)) 
		{
			printf("Parsing line - %s", line);
			if (opcode[0] == '\0' || opcode[0] == '#')
				continue;
			
			if (opcode[strlen(opcode) - 1] == ':') // This is label
			{
				if (rd[0] == '\0' || rd[0] == '#')
					continue;

				// Move pointers to their left pointer.
				strcpy(opcode, rd);
				strcpy(rd, rs);
				strcpy(rs, rt);
				strcpy(rt, immVal);
				strcpy(immVal, extraVal);
			}

			HandleCommand(opcode, rd, rs, rt, immVal);
		}
	}
}

void ExtractMemoryFile()
{
	for (int i = 0; i < MEMORY_ADDRESS_NUM; i++)
	{
		char* addressValue = memoryAddress[i] == NULL ? "00000000" : memoryAddress[i];
		fprintf(dmemFile, "%s\n", addressValue);
	}
}

void FreeAllMemory()
{
	for (int i = (int)labelsCounter - 1; i >= 0; i--)
	{
		free((LabelsLocations + i)->LabelName);
	}
	
	free(LabelsLocations);

	for (int i = 0; i < MEMORY_ADDRESS_NUM; i++)
	{
		if (memoryAddress[i] != NULL)
			free(memoryAddress[i]);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 4)
		return 1;
	
	asmFile = fopen(argv[1], "r");
	imemFile = fopen(argv[2], "w");
	dmemFile = fopen(argv[3], "w");
	if (asmFile == NULL || imemFile == NULL || dmemFile == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	InitMemoryAddress();
	
	ExtractLabels();
	ExtractCommands();
	ExtractMemoryFile();

	FreeAllMemory();
	
	fclose(asmFile);
	fclose(imemFile);
	fclose(dmemFile);
	return 0;
}
