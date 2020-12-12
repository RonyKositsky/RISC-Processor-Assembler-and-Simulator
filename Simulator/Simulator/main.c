#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Interrupts.h"
#include "IO\IOHandler.h"
#include "IO\Monitor.h"
#include "StaticDefinations.h"

FILE* TraceFile = NULL;

/*
 * hex value without '0x'
 */

void MemoryInit(FILE* dmemInFile)
{
	char line[10];
	int lineCounter = 0;
	while (fgets(line, 10, dmemInFile) != NULL) {
		Memory[lineCounter] = GetDecimalFromHex(line);
		lineCounter++;
	}

	while (lineCounter < MEMORY_SIZE) {
		Memory[lineCounter] = 0;
		lineCounter++;
	}
}

void InstructionInit(FILE* imemInFile)
{
	char line[7];
	int lineCounter = 0;
	while (fgets(line, 7, imemInFile) != NULL) {
		char opcodeBytes[3] = { line[0], line[1], '\0' };
		uint opcodeIndex = GetDecimalFromHex(opcodeBytes);
		char rdBytes[2] = { line[2], '\0' };
		uint rdIndex = GetDecimalFromHex(rdBytes);
		char rsBytes[2] = { line[3], '\0' };
		uint rsIndex = GetDecimalFromHex(rsBytes);
		char rtBytes[2] = { line[4], '\0' };
		uint rtIndex = GetDecimalFromHex(rtBytes);
		uint pcLocation = lineCounter;
		lineCounter++;

		char* name = (char*)calloc(6, sizeof(char));
		if (name == NULL)
			exit(1);
		
		RemoveLastChar(line);
		strcpy(name, line);
		
		uint immValue = 0, hasImm = 0;
		if (rdIndex == 1 || rtIndex == 1 || rsIndex == 1)
		{
			if (fgets(line, 7, imemInFile) != NULL)
			{
				immValue = GetDecimalFromHex2Comp(line);
			}
			lineCounter++;
			hasImm = 1;
		}
				
		InstructionCommand instruction = {
			pcLocation,
			OpcodeMapping[opcodeIndex],
			rdIndex,
			rsIndex,
			rtIndex,
			immValue,
			hasImm,
			name
		};

		if (InstructionCommands == NULL)
		{
			InstructionCommands = (InstructionCommand*)calloc(1, sizeof(InstructionCommand));
			if (InstructionCommands != NULL)
				*InstructionCommands = instruction;
			else
				exit(1); // TODO: What happen when fail to allocate memory?
		}
		else
		{
			InstructionCommands = realloc(InstructionCommands, (InstructionCounter + 1) * sizeof(InstructionCommand));
			if (InstructionCommands + InstructionCounter != NULL)
				*(InstructionCommands + InstructionCounter) = instruction;
			else
				exit(1); // TODO: What happen when fail to allocate memory?
		}
		InstructionCounter++;
	}

}

void GetTrace(InstructionCommand command)
{
	char pcBytes[4];
	GetHexValueOfConstant(command.PCLocation, pcBytes, 3);
	fprintf(TraceFile, "%s %d %s", pcBytes, ClockCycles, command.Name);

	for (int i = 0; i < NUMBER_OF_REGISTERS; i++) // TODO: Check if sign extension works for R1
	{
		char registerBytes[9];
		GetHexValueOfConstant(RegisterMapping[i].RegisterValue, registerBytes, 8);
		fprintf(TraceFile, " %s", registerBytes);
	}
	fputc('\n', TraceFile);
}

void MainLoop()
{
	while (ProgramCounter <= INSTRUCTION_COUNT)
	{
		// Check interrupt status
		uint irqStatus = GetIrqSignal();
		
		// Get instruction command
		InstructionCommand* command = GetInstructionCommand(ProgramCounter);
		if (command->HasImmediate)
		{
			RegisterMapping[1].RegisterValue = command->ImmValue;
		}
		
		// Get trace
		GetTrace(*command);

		// Increase PC
		ProgramCounter += IncreasePCAmount(*command);
		
		// Execute command
		command->opcode.OperationFunc(command->rd, command->rs, command->rt);

		// Handle IOs
		HandleIOs();

		// Increase ClockCycle
		ClockCycles += IncreasePCAmount(*command);
		
		// Interrupts execution
		ExecuteInterrupts();

		if (GetIrqSignal())
		{
			HandleInterrupt();
		}
	}
}

int main(int argc, char* argv[]) {
	/*if (argc != 14)
		return 1;*/
	
	FILE* imemInFile = fopen(argv[1], "r");
	FILE* dmemInFile = fopen(argv[2], "r");
	FILE* irq2InFile = fopen(argv[3], "r");

	// TODO: Fix files location
	FILE* monitorFile = fopen(argv[4], "w");
	
	TraceFile = fopen(argv[5], "w");

	if (imemInFile == NULL || dmemInFile == NULL || irq2InFile == NULL) {
		printf("Error! opening file");
		exit(1);
	}

	// Init Stage
	ProgramCounter = 0;
	ClockCycles = 0;
	MemoryInit(dmemInFile);
	InstructionInit(imemInFile);
	InitIrq2Array(irq2InFile);

	// Execute Stage
	MainLoop();


	// Finally
	PrintMonitorData(monitorFile);


	
	fclose(imemInFile);
	fclose(dmemInFile);
	fclose(irq2InFile);
	fclose(monitorFile);
}
