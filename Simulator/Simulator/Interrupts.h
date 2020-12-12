#ifndef INTERRUPTS
#define INTERRUPTS

#include <stdio.h>
#include "StaticDefinations.h"

uint InterruptBusy;

// Irq0

// Irq1

// Irq2
uint Irq2Counter;
uint* Irq2Array;
uint Irq2ArrayCount;

void InitIrq2Array(FILE* file);

// General
void InitInterrupts(FILE* irq2In);
uint GetIrqSignal(void);
void HandleInterrupt(void);
void ExecuteInterrupts(void);

#endif
