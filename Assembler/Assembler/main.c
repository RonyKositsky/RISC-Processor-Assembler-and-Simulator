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

/* Get label line number */
int GetLabelLine(char* label) {
	for (int i = 0; i < (int) labelsCounter; i++) {
		if (!strcmp(label, (*(LabelsLocations + i)).LabelName)) {
			return (int)LabelsLocations[i].LabelLine;
		}
	}
	return -1;
}

/*
 * Create new instance of Label and add it to array
 */
void AddLabelToArray(char* label, uint lineCounter)
{
	RemoveLastChar(label);
	Label labelStruct = { label, lineCounter };

	// If array empty
	if (LabelsLocations == NULL) {
		// Creation array
		LabelsLocations = (Label*)calloc(1, sizeof(Label));
		if (LabelsLocations != NULL)
			*LabelsLocations = labelStruct;
		else
		{
			printf("LabelsLocations allocation failed");
			exit(1);
		}
	}
	
	// Else reallocate array into new size
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

/*
 * Run through file and extract label location into LabelsLocations array
 */
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
			// Ignoring empty lines or .word line
			if (opcode[0] == '\0' || opcode[0] == '#' || opcode[0]=='.')
				continue;
			
			// If this is label
			if (opcode[strlen(opcode) - 1] == ':') { 
				AddLabelToArray(opcode, lineCounter);

				// Check if label on the same line as command, if so increase counter by command type
				if (rd[0] != '\0' && rd[0] != '#')
				{
					lineCounter += HasImmediate(rs, rt, immVal) ? 2 : 1;
				}
			}
			// Else increase counter by command type
			else
			{
				lineCounter += HasImmediate(rd, rs, rt) ? 2 : 1;
				free(opcode);
			}
		}	
	}
}

/*
 * Write word value into memory array at word location
 */
void HandleWordCommand(char* location, char* value)
{
	// Address
	int address = GetDecimalValueFromString(location);

	// Value
	int decValue = GetDecimalValueFromString(value);

	// Enter value to array
	memoryAddress[address] = (char*)calloc(9, sizeof(char));
	if (memoryAddress[address] != NULL)
		sprintf(memoryAddress[address], "%08X", decValue);
	else
	{
		printf("memoryAddress allocation failed");
		exit(1);
	}
}

/*
 * Build command code from assemble command
 */
void HandleCommand(char* opcode, char* rd, char* rs, char* rt, char* immVal)
{
	// if command is .word command
	if (!strcmp(opcode, ".word"))
	{
		HandleWordCommand(rd, rs);
		return;
	}

	// Removing comma if exists
	RemoveLastComma(rd); RemoveLastComma(rs); RemoveLastComma(rt);
	
	// Writing Opcode number and registers
	fprintf(imemFile, "%s%s%s%s\n",
		GetOpcodeNumber(opcode), GetRegisterNumber(rd), GetRegisterNumber(rs), GetRegisterNumber(rt));

	// Check if there is label in the immediate value
	if (InsideLabelList(immVal)) {
		// get label line number
		int lineLabel = GetLabelLine(immVal);
		// Write number to file as immediate 
		fprintf(imemFile, "%05X\n", lineLabel);
	}
	else {
		// if there is immediate value
		if (HasImmediate(rd, rs, rt)) {
			// Convert immVal to Hex
			int decValue = GetDecimalValueFromString(immVal);
			char hexValue[6];
			GetHexValueOfConstant(decValue, hexValue, 5);
			// Write number to file
			fprintf(imemFile, "%s\n", hexValue);
		}
	}
}

/* Extract command codes by assembly command */
void ExtractCommands() {
	char line[MAX_LINE_LENGTH];
	rewind(asmFile);

	// Run over all file lines
	while (fgets(line, MAX_LINE_LENGTH, asmFile) != NULL) {
		char opcode[20], rd[20], rs[20], rt[20], immVal[20], extraVal[20];
		opcode[0] = '\0';
		rd[0] = '\0';

		// Search for command line
		if (sscanf(line, "%s%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9]%*[ ,\t]%[a-zA-Z$0-9[-]]%*[ ,\t]%[a-zA-Z$0-9[-]]", opcode, rd, rs, rt, immVal, extraVal)) 
		{
			printf("Parsing line - %s", line);
			// Ignore line if is empty or a comment 
			if (opcode[0] == '\0' || opcode[0] == '#')
				continue;

			// If line has label
			if (opcode[strlen(opcode) - 1] == ':')
			{
				// If label alone in line then ignore
				if (rd[0] == '\0' || rd[0] == '#')
					continue;

				// Else, Move the pointers to their left pointer, so handle it like regular command
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

/*Write the memory to file*/
void ExtractMemoryFile()
{
	// Run over memory array and write data to file
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

	// Open files
	asmFile = fopen(argv[1], "r");
	imemFile = fopen(argv[2], "w");
	dmemFile = fopen(argv[3], "w");
	if (asmFile == NULL || imemFile == NULL || dmemFile == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	// Init Memory array
	InitMemoryAddress();

	// Extracting data from assembly file
	ExtractLabels();
	ExtractCommands();
	ExtractMemoryFile();

	FreeAllMemory();

	// Close all files
	fclose(asmFile);
	fclose(imemFile);
	fclose(dmemFile);
	return 0;
}
