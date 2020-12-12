#ifndef MONITOR
#define MONITOR
#define NUMBER_OF_PIXEL_X 352
#define NUMBER_OF_PIXEL_Y 288

#include <stdio.h>
#include "../StaticDefinations.h"

uint MonitorData[NUMBER_OF_PIXEL_X][NUMBER_OF_PIXEL_Y];

void InitMonitor(void);
void MonitorCommand(void);
void PrintMonitorData(FILE* monitorFile);
#endif
