#include "StaticDefinations.h"

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
	{"irq0enable", "0", 1, 0},
	{"irq1enable", "1", 1, 0},
	{"irq2enable", "2", 1, 0},
	{"irq0status", "3", 1, 0},
	{"irq1status", "4", 1, 0},
	{"irq2status", "5", 1, 0},
	{"irqhandler", "6", 12, 0},
	{"irqreturn", "7", 12, 0},
	{"clks", "8", 32, 0},
	{"leds", "9", 32, 0},
	{"reserved", "10", 32, 0},
	{"timerenable", "11", 1, 0},
	{"timercurrent", "12", 32, 0},
	{"timermax", "13", 32, 0},
	{"diskcmd", "14", 2, 0},
	{"disksector", "15", 7, 0},
	{"diskbuffer", "16", 12, 0},
	{"diskstatus", "17", 1, 0},
	{"monitorcmd", "18", 1, 0},
	{"monitorx", "19", 11, 0},
	{"monitory", "20", 10, 0},
	{"monitordata", "21", 8, 0}
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
	// TODO: Register $ra <- pc+(1|2)
	//RegisterMapping[15] = ProgramCounter + i
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
	// TODO: Check if need to take just low 10 bits
	ProgramCounter = IORegisterMapping[7].RegisterValue; // PC <- IORegister[7] (irqreturn - PC of interrupt return address, 12 bits) 
}
void In(uint rd, uint rs, uint rt) {
	uint ioIndex = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	RegisterMapping[rd].RegisterValue = IORegisterMapping[ioIndex].RegisterValue;
}
void Out(uint rd, uint rs, uint rt) {
	uint ioIndex = RegisterMapping[rs].RegisterValue + RegisterMapping[rt].RegisterValue;
	IORegisterMapping[ioIndex].RegisterValue = RegisterMapping[rd].RegisterValue;
}
void Halt(uint rd, uint rs, uint rt){} // TODO: Check how to implement

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
