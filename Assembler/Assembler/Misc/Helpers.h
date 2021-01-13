#ifndef HELPER
#define HELPER

typedef unsigned int uint;
typedef struct {
	char* LabelName;
	uint LabelLine;
}Label;

void RemoveLastChar(char* str);
void RemoveLastComma(char* str);
int GetDecimalFromHex(char* hexValue);
int GetDecimalValueFromString(char* str);
void GetHexValueOfConstant(uint num, char* hexVal, int numOfBytes);

int HasImmediate(char* rd, char* rs, char* rt);

#endif
