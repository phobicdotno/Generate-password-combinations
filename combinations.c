#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NUM_CHARS 62  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename

void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_file);

int main() {
    int min_len, max_len, allow_special_chars, allow_repeats;
    char wordlist_file[MAX_FILENAME_LEN];
    printf("Enter minimum length of combinations: ");
    scanf("%d", &min_len);
    printf("Enter maximum length of combinations: ");
    scanf("%d", &max_len);
    printf("Allow special characters? (0=no, 1=yes): ");
    scanf("%d", &allow_special_chars);
    printf("Allow repetitive characters? (0=no, 1=yes): ");
    scanf("%d", &allow_repeats);
    printf("Enter wordlist file name (optional): ");
    scanf("%s", wordlist_file);
    generate_combinations(min_len, max_len, allow_special_chars, allow_repeats, wordlist_file);
    return 0;
}

void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_file) {
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
    int num_words = 0;
    int* word_lengths = NULL;
    char** words = NULL;
    FILE* wordlist_fp = fopen(wordlist_file, "r");
    if (wordlist_fp != NULL) {
        int i, j;
        int max_word_length = 0;
        char line[256];
        while (fgets(line, 256, wordlist_fp) != NULL) {
            // Remove newline character
            line[strcspn(line, "\n")] = 0;
            // Allocate memory for word
            int word_length = strlen(line);
            if (word_length > 0) {
                num_words++;
                word_lengths = (int*) realloc(word_lengths, num_words * sizeof(int));
                word_lengths[num_words - 1] = word_length;
                words = (char**) realloc(words, num_words * sizeof(char*));
                words[num_words - 1] = (char*) malloc(word_length + 1);
                strcpy(words[num_words - 1], line);
                if (word_length > max_word_length) {
                    max_word_length = word_length;
                }
            }
        }
        fclose(wordlist_fp);
        // Pad shorter words with spaces
        for (i = 0; i < num_words; i++) {
            int pad_length = max_word_length - word_lengths[i];
            for (j = 0; j < pad_length; j++) {
                words[i][word_lengths[i] + j] = ' ';
            }
        }
    }
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
                if (!allow_repeats && j > 0 && combination[j] == combination[j - 1]) {
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
            time(&current_time);
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
}