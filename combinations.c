#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_CHARS 62  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename

void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_fp);
void generate_words(int* num_words, int** word_lengths, char*** words, FILE* wordlist_file);
int has_repeats(char* combination, int combination_len, int allow_repeats);
void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp, char** words, int num_words);

int main() {
    int min_len, max_len, allow_special_chars, allow_repeats;
    char wordlist_filename[MAX_FILENAME_LEN];
    printf("Enter minimum length of combinations: ");
    scanf("%d", &min_len);
    printf("Enter maximum length of combinations: ");
    scanf("%d", &max_len);
    printf("Allow special characters? (0=no, 1=yes): ");
    scanf("%d", &allow_special_chars);
    printf("Allow repetitive characters? (0=no, 1=yes): ");
    scanf("%d", &allow_repeats);
    printf("Enter wordlist file name (optional): ");
    scanf("%s", wordlist_filename);

    int num_words = 0;
    int* word_lengths = NULL;
    char** words = NULL;
    FILE* wordlist_file = NULL;
    if (strlen(wordlist_filename) > 0) {
        wordlist_file = fopen(wordlist_filename, "r");
        if (wordlist_file != NULL) {
            generate_words(&num_words, &word_lengths, &words, wordlist_file);
            fclose(wordlist_file);
        }
    }

    printf("Read %d words from file:\n", num_words);
    for (int i = 0; i < num_words; i++) {
        printf("%s\n", words[i]);
    }

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
