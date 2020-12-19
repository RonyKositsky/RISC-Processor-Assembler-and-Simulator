#include "Disk.h"

#include "../Misc/Files.h"
#include "../Misc/Registers.h"
#define DISK_OPERATION_TIME 1024

// TODO: Read & Write from file
void InitDiskMemory(FILE* diskInFile)
{
	DiskTimer = 0;

	/*char line[10];
	int lineCounter = 0;
	while (fgets(line, 10, DmemInFile) != NULL) {
		Memory[lineCounter] = GetDecimalFromHex(line);
		lineCounter++;
	}

	while (lineCounter < MEMORY_SIZE) {
		Memory[lineCounter] = 0;
		lineCounter++;
	}*/
}

void WriteDiskMemory(FILE* diskOutFile)
{
	/*for (uint i = 0; i < MEMORY_SIZE; i++)
	{
		char hexValue[9];
		GetHexValueOfConstant(Memory[i], hexValue, 8);

		fprintf(DmemOutFile, "%s\n", hexValue);
	}*/
}

void ReadSector()
{
	uint sector = IORegisterMapping[DISK_SECTOR].RegisterValue;
	IORegisterMapping[DISK_BUFFER].RegisterValue = DiskSectorMemory[sector];
}

void WriteSector()
{
	uint sector = IORegisterMapping[DISK_SECTOR].RegisterValue;
	DiskSectorMemory[sector] = IORegisterMapping[DISK_BUFFER].RegisterValue;
}

int DiskCommand(uint timerIncrement)
{
	if (DiskTimer < DISK_OPERATION_TIME)
	{
		DiskTimer += timerIncrement;
		return 0;
	}

	switch (IORegisterMapping[DISK_CMD].RegisterValue)
    {
		case 1:
			ReadSector();
			break;
		case 2:
			WriteSector();
			break;
		default:
			break;
    }
	
	DiskTimer = 0;
	return 1;
}


