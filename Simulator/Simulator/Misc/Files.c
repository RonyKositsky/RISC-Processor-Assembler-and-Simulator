#define _CRT_SECURE_NO_WARNINGS
#include "Files.h"

#include <string.h>

int OpenFiles(char* argv[])
{
	ImemInFile = fopen(argv[1], "r");
	DmemInFile = fopen(argv[2], "r");
	DiskInFile = fopen(argv[3], "r");
	Irq2InFile = fopen(argv[4], "r");
	DmemOutFile = fopen(argv[5], "w");
	RegOutFile = fopen(argv[6], "w");
	TraceFile = fopen(argv[7], "w");
	HwRegTraceFile = fopen(argv[8], "w");
	CyclesFile = fopen(argv[9], "w");
	LedsFile = fopen(argv[10], "w");
	MonitorFile = fopen(argv[11], "w");
	MonitorYuvFile = fopen(argv[12], "w");
	DiskOutFile = fopen(argv[13], "w");

	return (ImemInFile == NULL) ||
		(DmemInFile == NULL) ||
		(ImemInFile == NULL) ||
		(DmemInFile == NULL) ||
		(DiskInFile == NULL) ||
		(Irq2InFile == NULL) ||
		(DmemOutFile == NULL) ||
		(RegOutFile == NULL) ||
		(TraceFile == NULL) ||
		(HwRegTraceFile == NULL) ||
		(CyclesFile == NULL) ||
		(LedsFile == NULL) ||
		(MonitorFile == NULL) ||
		(MonitorYuvFile == NULL) ||
		(DiskOutFile == NULL);
}

void CloseFiles()
{
	fclose(ImemInFile);
	fclose(DmemInFile);
	fclose(DiskInFile);
	fclose(Irq2InFile);
	fclose(DmemOutFile);
	fclose(RegOutFile);
	fclose(TraceFile);
	fclose(HwRegTraceFile);
	fclose(CyclesFile);
	fclose(LedsFile);
	fclose(MonitorFile);
	fclose(MonitorYuvFile);
	fclose(DiskOutFile);
}

void WriteHwTrace(InstructionCommand command)
{
	char* mode = strcmp(command.opcode.OpcodeName, "in") ? "WRITE" : "READ";
	uint registerIndex = RegisterMapping[command.rs].RegisterValue + RegisterMapping[command.rt].RegisterValue;

	char hexValue[9];
	if (!strcmp(command.opcode.OpcodeName, "in"))
		GetHexValueOfConstant(IORegisterMapping[registerIndex].RegisterValue, hexValue, 8);
	else
		GetHexValueOfConstant(RegisterMapping[command.rd].RegisterValue, hexValue, 8);
		
	fprintf(HwRegTraceFile, "%d %s %s %s\n", 
		ClockCycles +1, mode, IORegisterMapping[registerIndex].RegisterName, hexValue);
}

void WriteTrace(InstructionCommand command)
{
	char pcBytes[4];
	GetHexValueOfConstant(command.PCLocation, pcBytes, 3);
	fprintf(TraceFile, "%s %s", pcBytes, command.Name);

	for (int i = 0; i < NUMBER_OF_REGISTERS; i++) // TODO: Check if sign extension works for R1
	{
		char registerBytes[9];
		GetHexValueOfConstant(RegisterMapping[i].RegisterValue, registerBytes, 8);
		fprintf(TraceFile, " %s", registerBytes);
	}
	fputc('\n', TraceFile);

	if (!strcmp(command.opcode.OpcodeName, "in") || !strcmp(command.opcode.OpcodeName, "out"))
		WriteHwTrace(command);
}

void WriteRegistersToFile()
{
	for (int i = 2; i < NUMBER_OF_REGISTERS; i++)
	{
		char registerBytes[9];
		GetHexValueOfConstant(RegisterMapping[i].RegisterValue, registerBytes, 8);
		fprintf(RegOutFile, "%s\n", registerBytes);
	}
}

void WriteCyclesToFile()
{
	fprintf(CyclesFile, "%d\n", ClockCycles);
	fprintf(CyclesFile, "%d\n", TotalInstructionsCommand);
}
