#include "Monitor.h"
#include "../Misc/Files.h"

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

void WriteMonitorData()
{
	for (int y = 0; y < NUMBER_OF_PIXEL_Y; y++)
	{
		for (int x = 0; x < NUMBER_OF_PIXEL_X; x++)
		{
			fprintf(MonitorFile, "%02X\n", MonitorData[x][y]);

			for(int i=0; i<8; i++)
			{
				unsigned char bit = (unsigned char)(((MonitorData[x][y]) & (1 << i)) != 0);
				fwrite(&bit, sizeof(bit),1 ,MonitorYuvFile);
			}			
		}
	}
}


