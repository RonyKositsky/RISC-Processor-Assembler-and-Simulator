#include "Interrupts.h"

#include <stdlib.h>
#include "Files.h"

uint Irq2Counter;
uint* Irq2Array;
uint Irq2ArrayCount;

void InitIrq2Array()
{
	char line[500];
	Irq2ArrayCount = 0;
	Irq2Counter = 0;
	
	while (fgets(line, 500, Irq2InFile) != NULL) {
		Irq2ArrayCount++;
	}

	rewind(Irq2InFile);
	Irq2Array = (uint*)calloc(Irq2ArrayCount, sizeof(uint));

	int index = 0;
	while (fgets(line, 500, Irq2InFile) != NULL) {
		Irq2Array[index] = (uint)atoi(line);
		index++;
	}
}

void CheckIrq0Status(uint incrementValue)
{
	if (IORegisterMapping[TIMER_ENABLE].RegisterValue)
	{
		if (IORegisterMapping[TIMER_CURRENT].RegisterValue >= IORegisterMapping[TIMER_MAX].RegisterValue)
		{
			IORegisterMapping[IRQ_0_STATUS].RegisterValue = 1;
			IORegisterMapping[TIMER_CURRENT].RegisterValue = 0;
		}
		else
			IORegisterMapping[TIMER_CURRENT].RegisterValue += incrementValue;
	}
}

void CheckIrq1Status()
{
	// TODO: Maybe put here the irq1status handling
}

void CheckIrq2Status()
{
	if (Irq2Counter < Irq2ArrayCount)
	{
		if (ClockCycles <= Irq2Array[Irq2Counter])
			return;

		IORegisterMapping[IRQ_2_STATUS].RegisterValue = 1;
		Irq2Counter++;
	}
}

void InitInterrupts()
{
	InterruptBusy = 0;
	InitIrq2Array();
}


uint GetIrqSignal()
{
	uint irq0enable = IORegisterMapping[IRQ_0_ENABLE].RegisterValue;
	uint irq1enable = IORegisterMapping[IRQ_1_ENABLE].RegisterValue;
	uint irq2enable = IORegisterMapping[IRQ_2_ENABLE].RegisterValue;

	uint irq0status = IORegisterMapping[IRQ_0_STATUS].RegisterValue;
	uint irq1status = IORegisterMapping[IRQ_1_STATUS].RegisterValue;
	uint irq2status = IORegisterMapping[IRQ_2_STATUS].RegisterValue;

	return (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status);
}

void HandleInterrupt()
{
	if (InterruptBusy)
		return;
	
	IORegisterMapping[IRQ_RETURN].RegisterValue = ProgramCounter;
	ProgramCounter = IORegisterMapping[IRQ_HANDLER].RegisterValue;
	InterruptBusy = 1;
}

void ExecuteInterrupts(uint incrementValue)
{
	CheckIrq0Status(incrementValue);
	CheckIrq1Status();
	CheckIrq2Status();
}
