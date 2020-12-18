#include "Led.h"
#include "../Misc/Files.h"

void WriteLEDStatus()
{
	char hexValue[9];
	GetHexValueOfConstant(LEDValue, hexValue, 8);

	// We take 'ClockCycles + 1' because the instruction out is immediate and we increase ClockCycles in 2 after this.
	fprintf(LedsFile, "%d %s\n", ClockCycles + 1, hexValue);
}


