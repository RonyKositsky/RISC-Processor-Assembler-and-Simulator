#ifndef DISK
#define DISK
#define SECTOR_NUMBER 128
#define SECTOR_SIZE 128

#include <stdio.h>
#include "../Misc/Helpers.h"

uint DiskTimer;
uint DiskSectorMemory[SECTOR_NUMBER][SECTOR_SIZE];

void InitDiskMemory(void);
void WriteDiskMemory(void);

int DiskCommand(uint timerIncrement);

#endif
