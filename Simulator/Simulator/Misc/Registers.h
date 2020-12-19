#ifndef REGISTERS
#define REGISTERS

#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_IO_REGISTERS 22
#define NUMBER_OF_OPCODES 22
#define INSTRUCTION_COUNT (InstructionCommands + InstructionCounter - 1)->PCLocation
#include "Helpers.h"

typedef enum
{
	IRQ_0_ENABLE,
	IRQ_1_ENABLE,
	IRQ_2_ENABLE,
	IRQ_0_STATUS,
	IRQ_1_STATUS,
	IRQ_2_STATUS,
	IRQ_HANDLER,
	IRQ_RETURN,
	CLKS,
	LEDS,
	RESERVED,
	TIMER_ENABLE,
	TIMER_CURRENT,
	TIMER_MAX,
	DISK_CMD,
	DISK_SECTOR,
	DISK_BUFFER,
	DISK_STATUS,
	MONITOR_CMD,
	MONITOR_X,
	MONITOR_Y,
	MONITOR_DATA
}IORegisterNames;

typedef struct
{
	char* RegisterName;
	char* RegisterNumber;
	uint RegisterValue;
}Register;
typedef struct
{
	char* RegisterName;
	char* RegisterNumber;
	int NumberOfBits;
	uint RegisterValue;
}IORegister;
typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(uint rd, uint rs, uint rt);
}Opcode;

typedef struct
{
	uint PCLocation;
	Opcode opcode;
	uint rd;
	uint rs;
	uint rt;
	int ImmValue;
	int HasImmediate;
	char* Name;
}InstructionCommand;

InstructionCommand* InstructionCommands;
uint InstructionCounter;

Register RegisterMapping[NUMBER_OF_REGISTERS];
IORegister IORegisterMapping[NUMBER_OF_IO_REGISTERS];
Opcode OpcodeMapping[NUMBER_OF_OPCODES];

void InstructionInit(void);
InstructionCommand* GetInstructionCommand(uint pc);
uint IncreasePCAmount(InstructionCommand command);

void FreeInstructionsCommandArray(void);

#endif
