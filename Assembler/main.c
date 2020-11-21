#include <stdio.h>
#include <string>
#define MAX_LINE_LENGTH 500

typedef unsigned int uint;
typedef struct {
	char* LableName;
	uint LabelLine;
}Label;

Label* LablesLocations = NULL;
uint labelsCounter = 0;

void ExtractLabels(FILE* file) {
	char* line = NULL;
	int lineCounter = 0;

	while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
	{
		char* opcode, rd, rs, rt, immVal;
		char* label;

		// Search for command line
		if (sscanf(line, "%s %s, %s, %s, %s", opcode, rd, rs, rt, immVal) != NULL) {
			if (!strcmp(rd, "$imm") || !strcmp(rs, "$imm") || !strcmp(rt, "$imm"))
				lineCounter += 2;
			else
				lineCounter++;
			continue;
		}

		// Search for label
		if (sscanf(line, "%s:", label) != NULL) {
			Label labelStruct = { label, lineCounter };
			if (labelsCounter == 0) {
				LablesLocations = (Label*)calloc(1, sizeof(Label));
				LablesLocations[0] = labelStruct;
			}
			else {
				LablesLocations = realloc(LablesLocations, (labelsCounter + 1) * sizeof(Label));
			}

			labelsCounter++;
		}
	}
}


int main(int argc, char* argv[]) {
	if (argc != 3)
		return 1;
	
	FILE* asmFile;
	if ((asmFile = fopen(argv[0], "r")) == NULL) {
		printf("Error! opening file");
		exit(1);
	}

}