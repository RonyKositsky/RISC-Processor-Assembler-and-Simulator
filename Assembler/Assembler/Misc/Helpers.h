#ifndef HELPER
#define HELPER

typedef unsigned int uint;
typedef struct {
	char* LabelName;
	uint LabelLine;
}Label;

/*Removing last character*/
void RemoveLastChar(char* str);
/*Removing last comma if exists*/
void RemoveLastComma(char* str);
/*Convert hex value to decimal (hex value with '0x')*/
int GetDecimalFromHex(char* hexValue);
/*Parse decimal value from string*/
int GetDecimalValueFromString(char* str);
/*Convert uint to hex string*/
void GetHexValueOfConstant(uint num, char* hexVal, int numOfBytes);

/*Check if command containing immediate value*/
int HasImmediate(char* rd, char* rs, char* rt);

#endif
