/*
This code generates all possible combinations of characters given a minimum and maximum length, and writes them to a series of text files. The code starts by prompting the user to enter the minimum and maximum length of the combinations, and then passes those values to a function called generate_combinations().
Inside the generate_combinations() function, the code first allocates memory for a filename and combination strings, and then loops through each combination length. For each combination length, the code calculates the total number of possible combinations and generates each combination sequentially. If the combination contains repeated characters, the code skips it. Otherwise, the code writes the combination to a text file.
If the file size reaches 1 GB, the code closes the current file and opens a new one, incrementing the file count by 1. Each file is named "generated0000.txt", "generated0001.txt", and so on. Additionally, the code writes a text file called "generated_files.txt", which contains a list of all the files generated, along with the first and last combination in each file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_CHARS 91  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename

void generate_combinations(int min_len, int max_len);

int main() {
    int min_len, max_len;
    printf("Enter minimum length of combinations: ");
    scanf("%d", &min_len);
    printf("Enter maximum length of combinations: ");
    scanf("%d", &max_len);
    generate_combinations(min_len, max_len);
    return 0;
}

void generate_combinations(int min_len, int max_len) {
    char* filename = (char*) malloc(MAX_FILENAME_LEN);
    char* combination = (char*) malloc(max_len + 1);
    memset(combination, 0, max_len + 1);
    int combination_len;
    FILE* fp = NULL;
    unsigned long long file_count = 0;
    FILE* list_fp = fopen("generated_files.txt", "w"); // open file to write generated files
    for (combination_len = min_len; combination_len <= max_len; combination_len++) {
        unsigned long long num_combinations = pow(NUM_CHARS, combination_len);
        int i;
        for (i = 0; i < num_combinations; i++) {
            int j;
            int has_repeats = 0;
            for (j = 0; j < combination_len; j++) {
                combination[j] = i / (int) pow(NUM_CHARS, j) % NUM_CHARS + 33;
                if (j > 0 && combination[j] == combination[j - 1]) {
                    has_repeats = 1;
                    break;
                }
            }
            if (!has_repeats) {
                if (fp == NULL) {
                    snprintf(filename, MAX_FILENAME_LEN, "generated%04llu.txt", file_count);
                    fp = fopen(filename, "w");
                    printf("Creating file: %s\n", filename);
                    fprintf(list_fp, "%s %s\n", combination, filename);
                }
                fprintf(fp, "%s\n", combination);
                if (ftell(fp) >= 1000000000) {  // 1 GB
                    fclose(fp);
                    fp = NULL;
                    file_count++;
                }
            }
        }
    }
    if (fp != NULL) {
        fclose(fp);
    }
    fprintf(list_fp, "Last generated combination: %s\n", combination);
    fclose(list_fp);
    free(combination);
    free(filename);
}
