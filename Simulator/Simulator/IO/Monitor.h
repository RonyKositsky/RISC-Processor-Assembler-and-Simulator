#ifndef MONITOR
#define MONITOR
#define NUMBER_OF_PIXEL_X 352
#define NUMBER_OF_PIXEL_Y 288

#include <stdio.h>
#include "../Misc/Helpers.h"

uint MonitorData[NUMBER_OF_PIXEL_X][NUMBER_OF_PIXEL_Y]; //Array representing the monitor data.

void InitMonitor(void);
void MonitorCommand(void);
void WriteMonitorData();
#endif
