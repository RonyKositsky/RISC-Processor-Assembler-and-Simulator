#ifndef INTERRUPTS
#define INTERRUPTS

#include "Helpers.h"

uint InterruptBusy;

// General
void InitInterrupts();
uint GetIrqSignal(void);
void HandleInterrupt(void);
void ExecuteInterrupts(uint incrementValue);

void FreeInterruptsMemory(void);

#endif
