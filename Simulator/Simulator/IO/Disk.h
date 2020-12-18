#ifndef DISK
#define DISK
#define SECTOR_NUMBER 128
#define SECTOR_SIZE 512

#include <stdio.h>
#include "../Misc/Helpers.h"

uint DiskTimer;
uint DiskSectorMemory[SECTOR_NUMBER];

void InitDiskMemory(FILE* diskInFile);
void WriteDiskMemory(FILE* diskOutFile);

int DiskCommand(uint timerIncrement);

#endif
