#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NUM_CHARS 62  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename

void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_file);
void generate_words(int* num_words, int** word_lengths, char*** words, FILE* wordlist_fp);
void pad_words(int num_words, int* word_lengths, char** words, int max_word_length);
int has_repeats(char* combination, int combination_len, int allow_repeats);
void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp, char** words, int num_words);

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

int has_repeats(char* combination, int combination_len, int allow_repeats) {
    if (allow_repeats) {
        return 0;
    }
    int i;
    for (i = 1; i < combination_len; i++) {
        if (combination[i] == combination[i - 1]) {
            return 1;
        }
    }
    return 0;
}

void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp, char** words, int num_words) {
    if (*fp == NULL) {
        // Create file name with format generatedXXXX.txt
        sprintf(filename, "generated%04llu.txt", *file_count);
        (*file_count)++;
        *fp = fopen(filename, "w");
        // Write filename to list file
        fprintf(list_fp, "%s\n", filename);
    }
    if (num_words > 0) {
        for (int i = 0; i < num_words; i++) {
            // Write out word from word list
            fprintf(*fp, "%s", words[i]);
            // Add remaining characters to end of word to create full combination
            for (int j = strlen(words[i]); j < combination_len; j++) {
                char c;
                do {
                    c = rand() % NUM_CHARS;
                    if (c < 10) {
                        c += '0';
                    } else if (c < 36) {
                        c += 'a' - 10;
                    } else {
                        c += 'A' - 36;
                    }
                } while (has_repeats(combination, j, allow_repeats) != 0 || (c < '0' || c > '9') && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && !allow_special_chars);
                combination[j] = c;
            }
            // Write out full combination
            fprintf(*fp, "%s\n", combination);
        }
    } else {
        // Write out full combination if no word list is provided
        for (int i = 0; i < combination_len; i++) {
            char c;
            do {
                c = rand() % NUM_CHARS;
                if (c < 10) {
                    c += '0';
                } else if (c < 36) {
                    c += 'a' - 10;
                } else {
                    c += 'A' - 36;
                }
            } while (has_repeats(combination, i, allow_repeats) != 0 || (c < '0' || c > '9') && (c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && !allow_special_chars);

            combination[i] = c;
        }
        fprintf(*fp, "%s\n", combination);
    }
}




void generate_words(int* num_words, int** word_lengths, char*** words, FILE* wordlist_fp) {
    int i, j;
    char line[256];
    while (fgets(line, 256, wordlist_fp) != NULL) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        // Allocate memory for word
        int word_length = strlen(line);
        if (word_length > 0) {
            (*num_words)++;
            *word_lengths = (int*) realloc(*word_lengths, (*num_words) * sizeof(int));
            (*word_lengths)[(*num_words) - 1] = word_length;
            *words = (char**) realloc(*words, (*num_words) * sizeof(char*));
            (*words)[(*num_words) - 1] = (char*) malloc(word_length + 1);
            strcpy((*words)[(*num_words) - 1], line);
        }
    }
}

void pad_words(int num_words, int* word_lengths, char** words, int max_word_length) {
    int i, j;
    // Pad shorter words with spaces
    for (i = 0; i < num_words; i++) {
        int pad_length = max_word_length - word_lengths[i];
        for (j = 0; j < pad_length; j++) {
            words[i][word_lengths[i] + j] = ' ';
        }
    }
}

void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char* wordlist_file) {
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
    if (wordlist_file != NULL) {
        FILE* wordlist_fp = fopen(wordlist_file, "r");
        if (wordlist_fp != NULL) {
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
                }
            }
            fclose(wordlist_fp);
        }
    }

    // Generate combinations with word list words and all possible combinations using remaining characters
    for (int i = 0; i < num_words; i++) {
        for (combination_len = min_len - word_lengths[i]; combination_len <= max_len - word_lengths[i]; combination_len++) {
            int num_combinations = pow(NUM_CHARS, combination_len);
            for (int j = 0; j < num_combinations; j++) {
                int combination_idx = 0;
                for (int k = 0; k < word_lengths[i]; k++) {
                    combination[combination_idx++] = words[i][k];
                }
                for (int k = 0; k < combination_len; k++) {
                    combination[combination_idx++] = 'a' + (j / (int) pow(NUM_CHARS, k)) % NUM_CHARS;
                }
                if (has_repeats(combination, combination_len + word_lengths[i], allow_repeats) == 0) {
                    void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp, char** words, int num_words);

                }
            }
        }
    }

    // Close file
    if (fp != NULL) {
        fclose(fp);
        printf("Generated %llu combinations in file %s.\n", file_count, filename);
    } else {
        printf("No combinations were generated.\n");
    }

    // Free memory
    free(filename);
    free(combination);
    for (int i = 0; i < num_words; i++) {
        free(words[i]);
    }
    free(words);
    free(word_lengths);

    // Close list
}