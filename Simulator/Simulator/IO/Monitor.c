#include "Monitor.h"
#include "../Misc/Files.h"

/*
* Init monitor data array to array of zeros(start with empty screen).
*/
void InitMonitor()
{
	// Init MonitorData array to 0;
	for (int i = 0; i < NUMBER_OF_PIXEL_X; i++)
		for (int j = 0; j < NUMBER_OF_PIXEL_Y; j++)
			MonitorData[i][j] = 0;
}

/*
* Writing new (x,y) pixel depending on the data on the IORegister.
*/
void MonitorCommand()
{
	uint x = IORegisterMapping[MONITOR_X].RegisterValue;
	uint y = IORegisterMapping[MONITOR_Y].RegisterValue;
	uint data = IORegisterMapping[MONITOR_DATA].RegisterValue;

	MonitorData[x][y] = data;
}

/*
*Writing the monitor data to files.
*One is to txt format and the second one to yuv format.
*/
void WriteMonitorData()
{
	for (int y = 0; y < NUMBER_OF_PIXEL_Y; y++)
	{
		for (int x = 0; x < NUMBER_OF_PIXEL_X; x++)
		{
			fprintf(MonitorFile, "%02X\n", MonitorData[x][y]);
			fwrite((unsigned char*)&MonitorData[x][y], sizeof(char), 1, MonitorYuvFile);
		}
	}
}


