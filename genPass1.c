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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LENGTH 100

void generateCombination(int minLength, int maxLength, char *charset, char *password, int index, int charsetLength, FILE *fp)
{
    if (index == maxLength) {
        fprintf(fp, "%s\n", password);
        return;
    }
    for (int i = 0; i < charsetLength; i++) {
        password[index] = charset[i];
        generateCombination(minLength, maxLength, charset, password, index + 1, charsetLength, fp);
        if (index == minLength - 1) {
            break;
        }
    }
}

int main()
{
    int minLength, maxLength;
    char charset[MAX_LENGTH];
    char password[MAX_LENGTH];

    printf("Enter the minimum length: ");
    scanf("%d", &minLength);
    printf("Enter the maximum length: ");
    scanf("%d", &maxLength);
    strcpy(charset, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    FILE *fp;
    fp = fopen("passwords.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    srand(time(NULL));
    generateCombination(minLength, maxLength, charset, password, 0, strlen(charset), fp);
    fclose(fp);
    return 0;
}
