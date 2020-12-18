#include "Disk.h"
#include "../Misc/Registers.h"
#define DISK_OPERATION_TIME 1024


// TODO: Read & Write from file
void InitDiskMemory(FILE* diskInFile)
{
	DiskTimer = 0;
}

void WriteDiskMemory(FILE* diskOutFile)
{
	
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


