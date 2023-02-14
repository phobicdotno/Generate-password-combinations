#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NUM_CHARS 62  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename

void generate_combinations(int min_len, int max_len, int allow_special_chars);

int main() {
    int min_len, max_len, allow_special_chars;
    printf("Enter minimum length of combinations: ");
    scanf("%d", &min_len);
    printf("Enter maximum length of combinations: ");
    scanf("%d", &max_len);
    printf("Allow special characters? (0=no, 1=yes): ");
    scanf("%d", &allow_special_chars);
    generate_combinations(min_len, max_len, allow_special_chars);
    return 0;
}

void generate_combinations(int min_len, int max_len, int allow_special_chars) {
    char* filename = (char*) malloc(MAX_FILENAME_LEN);
    char* combination = (char*) malloc(max_len + 1);
    memset(combination, 0, max_len + 1);
    int combination_len;
    FILE* fp = NULL;
    unsigned long long file_count = 0;
    FILE* list_fp = fopen("generated_files.txt", "w"); // open file to write generated files
    time_t current_time;
    time_t last_tick_time, last_minute_time;
    time(&last_tick_time);
    last_minute_time = last_tick_time;
    for (combination_len = min_len; combination_len <= max_len; combination_len++) {
        unsigned long long num_combinations = 1;
        int i;
        int num_chars = allow_special_chars ? 95 : 62;
        for (i = 0; i < combination_len; i++) {
            num_combinations *= num_chars;
        }
        for (i = 0; i < num_combinations; i++) {
            int j;
            int has_repeats = 0;
            for (j = 0; j < combination_len; j++) {
                combination[j] = i / (int) pow(num_chars, j) % num_chars;
                if (combination[j] < 10) {
                    combination[j] += 48;  // Map to ASCII digits
                } else if (combination[j] < 36) {
                    combination[j] += 55;  // Map to ASCII uppercase letters
                } else if (combination[j] < 62) {
                    combination[j] += 61;  // Map to ASCII lowercase letters
                } else if (allow_special_chars) {
                    if (combination[j] < 88) {
                        combination[j] += 7;  // Map to ASCII special characters
                    } else {
                        combination[j] += 13;  // Map to ASCII special characters
                    }
                } else {
                    has_repeats = 1;
                    break;
                }
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
            current_time = time(NULL);
            if (current_time - last_tick_time >= 10) {
                last_tick_time = current_time;
                printf(".");
                fflush(stdout);
            }
            if (current_time - last_minute_time >= 60) {
                last_minute_time = current_time;
                char* c = ctime(&current_time);
                printf("\nCurrent time: %s", c);
            }
        }
    }
    if (fp != NULL) {
        fclose(fp);
    }
    fclose(list_fp);
    free(combination);
    free(filename);
}
