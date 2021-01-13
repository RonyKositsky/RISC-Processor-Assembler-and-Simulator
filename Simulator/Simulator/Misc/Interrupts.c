#include "Interrupts.h"

#include <stdlib.h>
#include "Files.h"

uint Irq2Counter;
uint* Irq2Array = NULL;
uint Irq2ArrayCount;

/*
* Init array of all the times of the irq2 interrupt from the input file.
*/
void InitIrq2Array()
{
	char line[500];
	Irq2ArrayCount = 0;
	Irq2Counter = 0;
	
	while (fgets(line, 500, Irq2InFile) != NULL) {
		Irq2ArrayCount++;
	}

	if (Irq2ArrayCount == 0)
		return;
	
	rewind(Irq2InFile);
	Irq2Array = (uint*)calloc(Irq2ArrayCount, sizeof(uint));

	int index = 0;
	while (fgets(line, 500, Irq2InFile) != NULL) {
		Irq2Array[index] = (uint)atoi(line);
		index++;
	}
}

/*
* Check irq0 status. If irq0 is enabled, execute it every IORegisterMapping[TIMER_CURRENT] cycles.
*/
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


/*
* Check irq2 status. We hold array of all the interrupt times. If we pass the time in the current 
* array element the interrupt is triggered and we move our array pointer to the next element in the
* array.
* If we reach to the final array element the counter will be larger than the array size, and the 
* interrupt will not be triggered again.
*/
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

/*
* Init interrupts data for the start of the program.
*/
void InitInterrupts()
{
	InterruptBusy = 0;
	InitIrq2Array();
}

/*
* Get the irqsignal data. If it will be euqal to 1, interrupt signal will be sent.
*/
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

/*
* Handling the interrupt. If it is marked as busy, return.
* Else, save the PC, retrieve the interrupt PC and start handling it. Mark the interrupt as busy.
*/
void HandleInterrupt()
{
	if (InterruptBusy)
		return;
	
	IORegisterMapping[IRQ_RETURN].RegisterValue = ProgramCounter;
	ProgramCounter = IORegisterMapping[IRQ_HANDLER].RegisterValue;
	InterruptBusy = 1;
}

/*
* Execute the interrupt(checking the relevant interrupt by the signal).
* Pass incrementValue paramter to increase irq2 timer by the number of cycles we went forward.
*/
void ExecuteInterrupts(uint incrementValue)
{
	CheckIrq0Status(incrementValue);
	CheckIrq2Status();
}

/*
* Free the irq2 array memory allocation.
*/
void FreeInterruptsMemory()
{
	if (Irq2Array != NULL)
		free(Irq2Array);
}
