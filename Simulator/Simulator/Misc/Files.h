#ifndef FILES
#define FILES
#include <stdio.h>
#include "Registers.h"

FILE* ImemInFile;
FILE* DmemInFile; 
FILE* DiskInFile; // TODO: missing
FILE* Irq2InFile;
FILE* DmemOutFile; 
FILE* RegOutFile;
FILE* TraceFile;
FILE* HwRegTraceFile;
FILE* CyclesFile;
FILE* LedsFile;
FILE* MonitorFile;
FILE* MonitorYuvFile; // TODO: missing
FILE* DiskOutFile; // TODO: missing

int OpenFiles(char* argv[]);
void CloseFiles(void);

void WriteTrace(InstructionCommand command);
void WriteRegistersToFile(void);
void WriteCyclesToFile(void);

#endif
