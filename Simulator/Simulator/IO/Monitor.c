#include "Monitor.h"

void InitMonitor()
{
	// Init MonitorData array to 0;
	for (int i = 0; i < NUMBER_OF_PIXEL_X; i++)
		for (int j = 0; j < NUMBER_OF_PIXEL_Y; j++)
			MonitorData[i][j] = 0;
}

void MonitorCommand()
{
	uint x = IORegisterMapping[MONITOR_X].RegisterValue;
	uint y = IORegisterMapping[MONITOR_Y].RegisterValue;
	uint data = IORegisterMapping[MONITOR_DATA].RegisterValue;

	MonitorData[x][y] = data;
}

void PrintMonitorData(FILE* monitorFile)
{
	for (int y = 0; y < NUMBER_OF_PIXEL_Y; y++)
	{
		for (int x = 0; x < NUMBER_OF_PIXEL_X; x++)
		{
			char pixel[3];
			GetHexValueOfConstant(MonitorData[x][y], pixel, 2);
			fprintf(monitorFile, "%s\n", pixel);
		}
	}
}


