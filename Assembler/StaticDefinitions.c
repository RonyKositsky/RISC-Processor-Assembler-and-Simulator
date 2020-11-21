#define NUMBER_OF_REGISTERS 16
#define NUMBER_OF_OPCODES 22
#include <string>

typedef struct
{
	char* RegisterName;
	int RegisterNumber;
}Register;

typedef struct
{
	char* OpcodeName;
	int OpcodeNumber
}Opcode;

// Static definitions

Register RegisterMapping[NUMBER_OF_REGISTERS] = {
	{"$zero", 0},	// Constant zero
	{"$imm", 1},	// Sign extended immediate
	{"$v0", 2},		// Result value
	{"$a0", 3},		// Argument register
	{"$a1", 4},		// Argument register
	{"$t0", 5},		// Temporary register
	{"$t1", 6},		// Temporary register
	{"$t2", 7},		// Temporary register
	{"$t3", 8},		// Temporary register
	{"$s0", 9},		// Saved register
	{"$s1", 10},	// Saved register
	{"$s2", 11},	// Saved register
	{"$gp", 12},	// Global pointer (static data)
	{"$sp", 13},	// Stack pointer
	{"$fp", 14},	// Frame Pointer
	{"$ra", 15},	// Return address
};

Opcode OpcodeMapping[NUMBER_OF_OPCODES] = {
	{"add", 0},
	{"sub", 1},
	{"and", 2},
	{"or", 3},
	{"xor", 4},
	{"mul", 5},
	{"sll", 6},
	{"sra", 7},
	{"srl", 8},
	{"beq", 9},
	{"bne", 10},
	{"blt", 11},
	{"bgt", 12},
	{"ble", 13},
	{"bge", 14},
	{"jal", 15},
	{"lw", 16},
	{"sw", 17},
	{"reti", 18},
	{"in", 19},
	{"out", 20},
	{"halt", 21},
};

/*Getting register number by name.
Return -1 if not found
*/
int GetRegisterNumber(char* name) {
	for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
	{
		if (strcmp(RegisterMapping[i].RegisterName, name) == 0)
			return RegisterMapping[i].RegisterNumber;
	}

	return -1;
}

/*Getting opcode number by name.
Return -1 if not found
*/
int GetOpcodeNumber(char* name) {
	for (int i = 0; i < NUMBER_OF_OPCODES; i++)
	{
		if (strcmp(OpcodeMapping[i].OpcodeName, name) == 0)
			return OpcodeMapping[i].OpcodeNumber;
	}

	return -1;
}