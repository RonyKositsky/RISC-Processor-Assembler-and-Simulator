#ifndef INTERRUPTS
#define INTERRUPTS

#include "Helpers.h"

uint InterruptBusy; //Interrupt busy flag.

// General
void InitInterrupts();
uint GetIrqSignal(void);
void HandleInterrupt(void);
void ExecuteInterrupts(uint incrementValue);

void FreeInterruptsMemory(void);

#endif
