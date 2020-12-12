#include <stdlib.h>
#include <string.h>


#include "StaticDefinations.h"
#include "Interrupts.h"

// Helpers
int GetDecimalFromHex(char* hexValue)
{
	return (int)strtol(hexValue, NULL, 16);
}

int GetDecimalFromHex2Comp(char* hexValue)
{
	if (hexValue[0] == '0')
		return GetDecimalFromHex(hexValue);

	return ((GetDecimalFromHex(hexValue) & 0xFFFFF) << 12) >> 12;
}

void RemoveLastChar(char* str) {
	str[strlen(str) - 1] = '\0';
}

void GetHexValueOfConstant(uint num, char* hexVal, int numOfBytes) {
	const int base = 16;
	hexVal[numOfBytes] = '\0';
	int i = numOfBytes - 1;

	do {
		hexVal[i] = "0123456789ABCDEF"[num % base];
		i--;
		num /= base;
	} while (i >= 0);
}

// Static definitions
Register RegisterMapping[NUMBER_OF_REGISTERS] = {
	{"$zero", "0", 0},	// Constant zero
	{"$imm", "1", 0},		// Sign extended immediate
	{"$v0", "2", 0},		// Result value
	{"$a0", "3", 0},		// Argument register
	{"$a1", "4", 0},		// Argument register
	{"$t0", "5", 0},		// Temporary register
	{"$t1", "6", 0},		// Temporary register
	{"$t2", "7", 0},		// Temporary register
	{"$t3", "8", 0},		// Temporary register
	{"$s0", "9", 0},		// Saved register
	{"$s1", "A", 0},		// Saved register
	{"$s2", "B", 0},		// Saved register
	{"$gp", "C", 0},		// Global pointer (static data)
	{"$sp", "D", 0},		// Stack pointer
	{"$fp", "E", 0},		// Frame Pointer
	{"$ra", "F", 0},		// Return address
};

IORegister IORegisterMapping[NUMBER_OF_IO_REGISTERS] = {
	{IRQ_0_ENABLE, "0", 1, 0},
	{IRQ_1_ENABLE, "1", 1, 0},
	{IRQ_2_ENABLE, "2", 1, 0},
	{IRQ_0_STATUS, "3", 1, 0},
	{IRQ_1_STATUS, "4", 1, 0},
	{IRQ_2_STATUS, "5", 1, 0},
	{IRQ_HANDLER, "6", 12, 0},
	{IRQ_RETURN, "7", 12, 0},
	{CLKS, "8", 32, 0},
	{LEDS, "9", 32, 0},
	{RESERVED, "10", 32, 0},
	{TIMER_ENABLE, "11", 1, 0},
	{TIMER_CURRENT, "12", 32, 0},
	{TIMER_MAX, "13", 32, 0},
	{DISK_CMD, "14", 2, 0},
	{DISK_SECTOR, "15", 7, 0},
	{DISK_BUFFER, "16", 12, 0},
	{DISK_STATUS, "17", 1, 0},
	{MONITOR_CMD, "18", 1, 0},
	{MONITOR_X, "19", 11, 0},
	{MONITOR_Y, "20", 10, 0},
	{MONITOR_DATA, "21", 8, 0}
};

// Operation Functions
void Add(uint rd, uint rs, uint rt){
	RegisterMapping[rd].RegisterValue = 
		RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue; }
void Sub(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue - RegisterMapping[rt].RegisterValue; }
void And(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue & RegisterMapping[rt].RegisterValue; }
void Or(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue | RegisterMapping[rt].RegisterValue; }
void Xor(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue ^ RegisterMapping[rt].RegisterValue; }
void Multiply(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue * RegisterMapping[rt].RegisterValue; }
void LogicShiftLeft(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue << RegisterMapping[rt].RegisterValue; }
void ArithmeticShiftRight(uint rd, uint rs, uint rt){} // TODO:
void LogicShiftRight(uint rd, uint rs, uint rt) {
	RegisterMapping[rd].RegisterValue =
		RegisterMapping[rs].RegisterValue >> RegisterMapping[rt].RegisterValue; }
void BranchEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue == RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchNotEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue != RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchLessThen(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue < RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchGraterThen(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue > RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchLessEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue <= RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void BranchGraterEqual(uint rd, uint rs, uint rt) {
	uint condition = RegisterMapping[rs].RegisterValue >= RegisterMapping[rt].RegisterValue;
	if (condition)
		ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF; // Taking the low 10 bits
}
void JumpAndLink(uint rd, uint rs, uint rt) {
	RegisterMapping[15].RegisterValue = ProgramCounter;
	ProgramCounter = RegisterMapping[rd].RegisterValue & 0x1FF;
}
void LoadWord(uint rd, uint rs, uint rt) {
	uint address = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	RegisterMapping[rd].RegisterValue = Memory[address];
}
void StoreWord(uint rd, uint rs, uint rt) {
	uint address = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	Memory[address] = RegisterMapping[rd].RegisterValue;
}
void ReturnInterrupt(uint rd, uint rs, uint rt) {
	ProgramCounter = IORegisterMapping[IRQ_RETURN].RegisterValue;
	InterruptBusy = 0;
}
void In(uint rd, uint rs, uint rt) {
	uint ioIndex = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	RegisterMapping[rd].RegisterValue = IORegisterMapping[ioIndex].RegisterValue;
}
void Out(uint rd, uint rs, uint rt) {
	uint ioIndex = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	IORegisterMapping[ioIndex].RegisterValue = RegisterMapping[rd].RegisterValue;
}
void Halt(uint rd, uint rs, uint rt) // TODO: Check it works
{
	ProgramCounter = INSTRUCTION_COUNT + 1;
}

Opcode OpcodeMapping[NUMBER_OF_OPCODES] = {
	{"add", "00", Add},
	{"sub", "01", Sub},
	{"and", "02", And},
	{"or", "03", Or},
	{"xor", "04", Xor},
	{"mul", "05", Multiply},
	{"sll", "06", LogicShiftLeft},
	{"sra", "07", ArithmeticShiftRight},
	{"srl", "08", LogicShiftRight},
	{"beq", "09", BranchEqual},
	{"bne", "0A", BranchNotEqual},
	{"blt", "0B", BranchLessThen},
	{"bgt", "0C", BranchGraterThen},
	{"ble", "0D", BranchLessEqual},
	{"bge", "0E", BranchGraterEqual},
	{"jal", "0F", JumpAndLink},
	{"lw", "10", LoadWord},
	{"sw", "11", StoreWord},
	{"reti", "12", ReturnInterrupt},
	{"in", "13", In},
	{"out", "14", Out},
	{"halt", "15", Halt},
};

InstructionCommand* GetInstructionCommand(uint pc)
{
	for (uint i = 0; i < InstructionCounter; i++)
	{
		if (InstructionCommands[i].PCLocation == pc)
			return &InstructionCommands[i];
	}
	
	return NULL;
}

uint IncreasePCAmount(InstructionCommand command)
{
	return command.HasImmediate ? 2 : 1;
}
