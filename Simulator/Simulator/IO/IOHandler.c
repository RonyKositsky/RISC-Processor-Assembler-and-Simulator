#include "IOHandler.h"
#include "StaticDefinations.h"
#include "Monitor.h"

void HandleIOs()
{
	if (IORegisterMapping[MONITOR_CMD].RegisterValue)
	{
		// TODO: Should we set register back to 0
		MonitorCommand();
		IORegisterMapping[MONITOR_CMD].RegisterValue = 0;
	}
}

