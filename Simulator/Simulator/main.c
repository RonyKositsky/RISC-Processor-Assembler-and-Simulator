#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>


#include "IO/Disk.h"
#include "Misc/Interrupts.h"
#include "IO/IOHandler.h"
#include "IO/Monitor.h"
#include "Misc/Registers.h"
#include "Misc/Files.h"
#include "Misc/Memory.h"
#include "Misc/Helpers.h"

/*Initiate all relevant modules*/
void Init()
{
	ProgramCounter = 0;
	ClockCycles = 0;
	
	MemoryInit();
	InitDiskMemory();
	InstructionInit();
	InitInterrupts();
}

/*
* Main loop of the simulator. Its operation sequence:
* -Handle interrupt.
* -Fetch command.
* -Increase PC and timer.
* -Execute.
*/
void MainLoop()
{
	while (ProgramCounter <= INSTRUCTION_COUNT)
	{
		// Get instruction command
		InstructionCommand* command = GetInstructionCommand(ProgramCounter);
		if (command->HasImmediate)
		{
			RegisterMapping[1].RegisterValue = command->ImmValue;
		}
		
		// Writing current trace
		WriteTrace(*command);

		// Increase PC -
		// PC update is on this location because the jump/branch commands need the updated value
		uint incrementPCValue = IncreasePCAmount(*command);
		ProgramCounter += incrementPCValue;
		
		// Execute command
		command->opcode.OperationFunc(command->rd, command->rs, command->rt);

		// Handle IOs
		HandleIOs(incrementPCValue);

		// Increase ClockCycle
		ClockCycles += incrementPCValue;

		// Interrupts execution
		ExecuteInterrupts(incrementPCValue);

		// Check interrupts status
		if (GetIrqSignal())
		{
			HandleInterrupt();
		}

		// Increase instructions command
		TotalInstructionsCommand++;
	}
}

/*
* All the closing and memory freeing before exiting the program.
*/
void Exit()
{
	// Write out files
	WriteMemoryToFile();
	WriteMonitorData();
	WriteCyclesToFile();
	WriteRegistersToFile();
	WriteDiskMemory();
	
	CloseFiles();
	
	// Free instructionCommand memory
	FreeInstructionsCommandArray();
	// Free Irq2Array
	FreeInterruptsMemory();
}

int main(int argc, char* argv[]) {
	if (argc != 14)
		return 1;

	if (OpenFiles(argv))
	{
		printf("Error! opening file");
		exit(1);
	}

	// Init Stage
	Init();

	// Execute Stage
	MainLoop();

	// Exit Stage
	Exit();
}
