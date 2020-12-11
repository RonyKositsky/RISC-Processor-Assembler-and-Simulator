#ifndef STATIC_DEFINITION
#define STATIC_DEFINITION

#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_IO_REGISTERS 22
#define NUMBER_OF_OPCODES 22
#define MEMORY_SIZE 4096

typedef unsigned int uint;
typedef struct // TODO: Remove name if unnecessary 
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
typedef struct // TODO: Remove name if unnecessary 
{
	char* OpcodeName;
	char* OpcodeNumber;
	void (*OperationFunc)(uint rd, uint rs, uint rt);
}Opcode;

uint ProgramCounter;
uint Memory[MEMORY_SIZE];

Register RegisterMapping[NUMBER_OF_REGISTERS];
IORegister IORegisterMapping[NUMBER_OF_IO_REGISTERS];
Opcode OpcodeMapping[NUMBER_OF_OPCODES];

#endif
