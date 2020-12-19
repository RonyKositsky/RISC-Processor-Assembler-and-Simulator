#include "StaticDefinitions.h"
#include <string.h>


// Static definitions

Register RegisterMapping[NUMBER_OF_REGISTERS] = {
	{"$zero", "0"},	// Constant zero
	{"$imm", "1"},	// Sign extended immediate
	{"$v0", "2"},	// Result value
	{"$a0", "3"},	// Argument register
	{"$a1", "4"},	// Argument register
	{"$t0", "5"},	// Temporary register
	{"$t1", "6"},	// Temporary register
	{"$t2", "7"},	// Temporary register
	{"$t3", "8"},	// Temporary register
	{"$s0", "9"},	// Saved register
	{"$s1", "A"},	// Saved register
	{"$s2", "B"},	// Saved register
	{"$gp", "C"},	// Global pointer (static data)
	{"$sp", "D"},	// Stack pointer
	{"$fp", "E"},	// Frame Pointer
	{"$ra", "F"},	// Return address
};

Opcode OpcodeMapping[NUMBER_OF_OPCODES] = {
	{"add", "00"},
	{"sub", "01"},
	{"and", "02"},
	{"or", "03"},
	{"xor", "04"},
	{"mul", "05"},
	{"sll", "06"},
	{"sra", "07"},
	{"srl", "08"},
	{"beq", "09"},
	{"bne", "0A"},
	{"blt", "0B"},
	{"bgt", "0C"},
	{"ble", "0D"},
	{"bge", "0E"},
	{"jal", "0F"},
	{"lw", "10"},
	{"sw", "11"},
	{"reti", "12"},
	{"in", "13"},
	{"out", "14"},
	{"halt", "15"},
};

/*Getting register hex value number by name.
Return -1 if not found
*/
char* GetRegisterNumber(char* name) {
	for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
	{
		if (strcmp(RegisterMapping[i].RegisterName, name) == 0)
			return RegisterMapping[i].RegisterNumber;
	}

	return -1;
}

/*Getting opcode hex value number by name.
Return -1 if not found
*/
char* GetOpcodeNumber(char* name) {
	for (int i = 0; i < NUMBER_OF_OPCODES; i++)
	{
		if (strcmp(OpcodeMapping[i].OpcodeName, name) == 0)
			return OpcodeMapping[i].OpcodeNumber;
	}

	return -1;
}