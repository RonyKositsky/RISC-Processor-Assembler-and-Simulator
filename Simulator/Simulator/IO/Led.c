#include "Led.h"
#include "../Misc/Files.h"

void WriteLEDStatus()
{
	// We take 'ClockCycles + 1' because the instruction out is immediate and we increase ClockCycles in 2 after this.
	fprintf(LedsFile, "%d %08X\n", ClockCycles + 1, LEDValue);
}


