#ifndef STATIC_DEFINITIONS
#define STATIC_DEFINITIONS
#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_OPCODES 22

typedef struct
{
	char* RegisterName;
	char* RegisterNumber;
}Register;

typedef struct
{
	char* OpcodeName;
	char* OpcodeNumber;
}Opcode;

Register RegisterMapping[NUMBER_OF_REGISTERS];
Opcode OpcodeMapping[NUMBER_OF_OPCODES];

char* GetRegisterNumber(char* name);
char* GetOpcodeNumber(char* name);

#endif
