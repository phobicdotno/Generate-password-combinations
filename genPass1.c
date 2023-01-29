<<<<<<< HEAD
//code that generates all possible combinations of all 95 printable ASCII characters (32-126) with the last three 
//characters are numbers and accepts four input parameters, minLength, maxLength, MBPerFile and path to specify the range 
//of lengths of the combinations, the maximum size of the file and the path to save the files:

/*
The function generateCombinations generates all possible combinations of the printable ASCII characters from 32 to 126 and the last three characters are numbers. 
It accepts four input parameters: minLength, maxLength, MBPerFile and path to specify the range of lengths of the 
combinations, the maximum size of the file and the path to save the files. 
The main function asks the user to input the values of these parameters and then calls the generateCombinations function. 
The generateCombinations function uses recursion to generate all possible combinations and writes them to the file in the specified path.
*/


=======
>>>>>>> parent of 409d0cf (Revert "Works, but to few chars used")
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const int num_characters = strlen(characters);

void generateCombinations(FILE *fp, int minLength, int maxLength, int currentLength, char *currentCombination) {
    if (currentLength >= minLength) {
        fprintf(fp, "%s\n", currentCombination);
    }

    if (currentLength == maxLength) {
        return;
    }

    for (int i = 0; i < num_characters; i++) {
        currentCombination[currentLength] = characters[i];
        generateCombinations(fp, minLength, maxLength, currentLength + 1, currentCombination);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: %s <min length> <max length> <max file size (MB)> <output file>\n", argv[0]);
        return 1;
    }

    int minLength = atoi(argv[1]);
    int maxLength = atoi(argv[2]);
    int maxFileSize = atoi(argv[3]);
    char *outputFile = argv[4];

    FILE *fp;
    fp = fopen(outputFile, "w");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char *currentCombination = malloc((maxLength + 1) * sizeof(char));
    currentCombination[maxLength] = '\0';

    generateCombinations(minLength, maxLength, maxFileSize, fp, 0);


    fclose(fp);

    return 0;
}
