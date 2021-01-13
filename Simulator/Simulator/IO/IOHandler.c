#include "IOHandler.h"
#include "Monitor.h"
#include "Disk.h"
#include "Led.h"
#include "../Misc/Registers.h"

/*
* Handling all the IO after each cycle in the main loop.
* The IO will be handled depends on the relevant IORegister status.
* incrementValue - timer cycles which increased during the main loop
*(1 or 2 depending if we recieved imm command).
*/
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

	//Increase CLKS register.
	IORegisterMapping[CLKS].RegisterValue += incrementValue;
}

