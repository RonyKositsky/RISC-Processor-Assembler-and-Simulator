#ifndef STATIC_DEFINITION
#define STATIC_DEFINITION

#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_IO_REGISTERS 22
#define NUMBER_OF_OPCODES 22
#define MEMORY_SIZE 4096
#define INSTRUCTION_COUNT (InstructionCommands + InstructionCounter - 1)->PCLocation

typedef unsigned int uint;
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

typedef struct // TODO: Remove name if unnecessary 
{
	char* RegisterName;
	char* RegisterNumber;
	uint RegisterValue;
}Register;
typedef struct
{
	IORegisterNames RegisterName;
	char* RegisterNumber;
	int NumberOfBits;
	uint RegisterValue;
}IORegister;
typedef struct // TODO: Remove name if unnecessary 
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

uint ProgramCounter;
InstructionCommand* InstructionCommands;
uint InstructionCounter;
uint ClockCycles;
uint Memory[MEMORY_SIZE];

Register RegisterMapping[NUMBER_OF_REGISTERS];
IORegister IORegisterMapping[NUMBER_OF_IO_REGISTERS];
Opcode OpcodeMapping[NUMBER_OF_OPCODES];

InstructionCommand* GetInstructionCommand(uint pc);
uint IncreasePCAmount(InstructionCommand command);

// Helpers
int GetDecimalFromHex(char* hexValue);
int GetDecimalFromHex2Comp(char* hexValue);
void RemoveLastChar(char* str);
void GetHexValueOfConstant(uint num, char* hexVal, int numOfBytes);

#endif
