#include "Disk.h"

#include "../Misc/Files.h"
#include "../Misc/Memory.h"
#include "../Misc/Registers.h"
#define DISK_OPERATION_TIME 1024 //Definition of disk cycle time.

/*
*Init the disk memory array.
*/
void InitDiskMemory()
{
	DiskTimer = 0;

	char line[10];
	int lineCounter = 0;
	while (fgets(line, 10, DiskInFile) != NULL) {
		int sector = lineCounter / SECTOR_NUMBER;
		int sectorLocation = lineCounter % SECTOR_SIZE;
		DiskSectorMemory[sector][sectorLocation] = GetDecimalFromHex(line);
		lineCounter++;
	}

	while (lineCounter < SECTOR_NUMBER * SECTOR_SIZE) {
		int sector = lineCounter / SECTOR_NUMBER;
		int sectorLocation = lineCounter % SECTOR_SIZE;
		DiskSectorMemory[sector][sectorLocation] = 0;
		lineCounter++;
	}
}

/*
* Write disk memory to file.
*/
void WriteDiskMemory()
{
	for (int sector = 0; sector < SECTOR_NUMBER; sector++)
	{
		for (int sectorLocation = 0; sectorLocation < SECTOR_SIZE; sectorLocation++)
		{
			fprintf(DiskOutFile, "%08X\n", DiskSectorMemory[sector][sectorLocation]);
		}
	}
}

/*
* Read disk operation. Read complete sector.
*/
void ReadSector()
{
	uint sector = IORegisterMapping[DISK_SECTOR].RegisterValue;
	uint memoryPointer = IORegisterMapping[DISK_BUFFER].RegisterValue;

	for (int sectorLocation = 0; sectorLocation < SECTOR_SIZE; sectorLocation++)
	{
		Memory[memoryPointer + sectorLocation] = DiskSectorMemory[sector][sectorLocation];
	}
}

/*
* Write disk operation. Write complete sector.
*/
void WriteSector()
{
	uint sector = IORegisterMapping[DISK_SECTOR].RegisterValue;
	uint memoryPointer = IORegisterMapping[DISK_BUFFER].RegisterValue;

	for (int sectorLocation = 0; sectorLocation < SECTOR_SIZE; sectorLocation++)
	{
		DiskSectorMemory[sector][sectorLocation] = Memory[memoryPointer + sectorLocation];
	}
}

/*
* Disk command. Check if the last command has ended.
* If not, increase clock cycles and return.
*/
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


