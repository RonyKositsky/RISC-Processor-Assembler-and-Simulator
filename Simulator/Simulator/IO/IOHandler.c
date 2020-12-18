#include "IOHandler.h"
#include "Monitor.h"
#include "Disk.h"
#include "Led.h"
#include "../Misc/Registers.h"

void HandleIOs(uint incrementValue)
{
	if (IORegisterMapping[MONITOR_CMD].RegisterValue)
	{
		MonitorCommand();
		IORegisterMapping[MONITOR_CMD].RegisterValue = 0;
	}

	// If DISK_CMD is read or write
	if (IORegisterMapping[DISK_CMD].RegisterValue != 0)
	{
		IORegisterMapping[DISK_STATUS].RegisterValue = 1;
		
		// Disk command
		if (!DiskCommand(incrementValue))
			return;

		IORegisterMapping[DISK_CMD].RegisterValue = 0;
		IORegisterMapping[DISK_STATUS].RegisterValue = 0;
		IORegisterMapping[IRQ_1_STATUS].RegisterValue = 1;
	}

	// Handle Leds
	if (IORegisterMapping[LEDS].RegisterValue != LEDValue)
	{
		LEDValue = IORegisterMapping[LEDS].RegisterValue;
		WriteLEDStatus();
	}
}

