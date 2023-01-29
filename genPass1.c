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
