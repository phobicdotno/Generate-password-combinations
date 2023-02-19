#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define NUM_CHARS 62  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename
#define MAX_WORD_LEN 100  // Maximum length of word
#define MAX_WORDLIST_LEN 1000000  // Maximum number of words in wordlist
#define MAX_NUM_WORDS 1000  // Maximum number of words in wordlist
unsigned long long file_count = 0;
FILE* fp;

// Function prototypes
int has_repeats(char* combination, int combination_len, int allow_repeats);
void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp);
void print_progress_and_time(time_t current_time, time_t* last_tick_time, time_t* last_minute_time);
void print_current_time(time_t current_time);
void free_memory(int num_words, int* word_lengths, char** words);
void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_file);
void generate_word_combinations(int combination_len, int max_len, char** words, int num_words, int allow_repeats, FILE* fp, char* filename, unsigned long long* file_count, FILE* list_fp, int* used_words);
void free_words(char** words, int num_words);
void generate_word_combinations_helper(int combination_len, int max_len, char** words, int num_words, int allow_repeats, FILE* fp, char* filename, unsigned long long* file_count, FILE* list_fp, char* combination, int index, int* used_words);
char** load_words(char* filename, int* num_words);


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
    printf("Enter wordlist file name (optional, enter 0 if not using): ");
    scanf("%s", wordlist_file);

    // check if the user wants to use a wordlist or not
    if (strcmp(wordlist_file, "0") == 0) {
        generate_combinations(min_len, max_len, allow_special_chars, allow_repeats, NULL);
    } else {
        generate_combinations(min_len, max_len, allow_special_chars, allow_repeats, wordlist_file);
    }

    return 0;
}



void free_words(char** words, int num_words) {
    for (int i = 0; i < num_words; i++) {
        free(words[i]);
    }
    free(words);
}

// Free memory allocated for wordlist
void free_memory(int num_words, int* word_lengths, char** words) {
    int i;
    for (i = 0; i < num_words; i++) {
        free(words[i]);
    }
    free(word_lengths);
    free(words);
}

void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_file) {
    // Open output file
    char filename[MAX_FILENAME_LEN];
    sprintf(filename, "combinations-%d-%d%s.txt", min_len, max_len, allow_special_chars ? "-special" : "");
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open output file %s.\n", filename);
        exit(1);
    }

    // Generate combinations using words from dictionary
    if (wordlist_file[0] == '\0') {
        generate_word_combinations(max_len, max_len, NULL, 0, allow_repeats, fp, filename, &file_count, NULL, NULL);
    }

    // Generate combinations using words from wordlist
    else {
        // Load words from file
        FILE* list_fp = fopen(wordlist_file, "r");
        if (list_fp == NULL) {
            fprintf(stderr, "Error: Could not open wordlist file %s.\n", wordlist_file);
            exit(1);
        }
        char** words;
        int num_words;
        words = load_words(wordlist_file, &num_words);
        fclose(list_fp);
        int* used_words = calloc(num_words, sizeof(int));
        if (used_words == NULL) {
            fprintf(stderr, "Error: Could not allocate memory.\n");
            exit(1);
        }
        for (int i = min_len; i <= max_len; i++) {
            memset(used_words, 0, num_words * sizeof(int));
            generate_word_combinations(i, max_len, words, num_words, allow_repeats, fp, filename, &file_count, NULL, used_words);
        }
        free(used_words);
        free_words(words, num_words);
    }

    // Close output file
    fclose(fp);
}

void generate_word_combinations(int combination_len, int max_len, char** words, int num_words, int allow_repeats, FILE* fp, char* filename, unsigned long long* file_count, FILE* list_fp, int* used_words) {
    // Allocate memory for the combination array
    char* combination = (char*) malloc(max_len + 1);
    combination[0] = '\0';

    // Generate all combinations of the specified length
    generate_word_combinations_helper(combination_len, max_len, words, num_words, allow_repeats, fp, filename, file_count, list_fp, combination, 0, used_words);

    // Free memory for the combination array
    free(combination);
}

void generate_word_combinations_helper(int combination_len, int max_len, char** words, int num_words, int allow_repeats, FILE* fp, char* filename, unsigned long long* file_count, FILE* list_fp, char* combination, int index, int* used_words) {
    if (strlen(combination) >= max_len) {
        write_combination(combination, max_len, allow_repeats, 0, &fp, filename, file_count, list_fp);
        return;
    }

    for (int i = 0; i < num_words; i++) {
        if (!allow_repeats && used_words[i]) {
            continue;
        }

        if (strlen(words[i]) + index > max_len) {
            continue;
        }

        char* new_combination = (char*) malloc(max_len + 1);
        strcpy(new_combination, combination);
        strcat(new_combination, words[i]);

        int* new_used_words = NULL;
        if (!allow_repeats) {
            new_used_words = malloc(num_words * sizeof(int));
            if (new_used_words == NULL) {
                fprintf(stderr, "Error: Could not allocate memory.\n");
                exit(1);
            }
            memcpy(new_used_words, used_words, num_words * sizeof(int));
            new_used_words[i] = 1;
        }

        generate_word_combinations_helper(combination_len, max_len, words, num_words, allow_repeats, fp, filename, file_count, list_fp, new_combination, index + strlen(words[i]), new_used_words);

        free(new_combination);
        if (new_used_words != NULL) {
            free(new_used_words);
        }
    }
}

// Check if a combination of characters has repeating characters
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

char** load_words(char* filename, int* num_words) {
    // Open the file for reading
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Unable to open file: %s\n", filename);
        exit(1);
    }

    // Allocate memory for the words and word lengths arrays
    char** words = (char**) malloc(MAX_NUM_WORDS * sizeof(char*));
    int* word_lengths = (int*) malloc(MAX_NUM_WORDS * sizeof(int));
    int i = 0;

    // Read each line of the file and store it in the words array
    char line[MAX_WORD_LEN];
    while (fgets(line, MAX_WORD_LEN, fp) != NULL) {
        // Remove the newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';
        //printf("%s\n", line);  // Debug check

        // Allocate memory for the current word
        int word_length = strlen(line);
        char* word = (char*) malloc(word_length + 1);
        strcpy(word, line);

        // Add the word and its length to the arrays
        if (i >= MAX_NUM_WORDS) {
            printf("Too many words in file: %s\n", filename);
            exit(1);
        }
        words[i] = word;
        word_lengths[i] = word_length;
        i++;
    }

    // Close the file
    fclose(fp);

    // Set the number of words and resize the arrays to the actual number of words
    *num_words = i;
    words = (char**) realloc(words, (*num_words) * sizeof(char*));
    word_lengths = (int*) realloc(word_lengths, (*num_words) * sizeof(int));

    // Return the words array
    return words;
}

// Print current time
void print_current_time(time_t current_time) {
    char* c = ctime(&current_time);
    printf("\nCurrent time: %s", c);
}

// Print progress and time every 10 seconds, and print current time every minute
void print_progress_and_time(time_t current_time, time_t* last_tick_time, time_t* last_minute_time) {
    if (current_time - *last_tick_time >= 10) {
        *last_tick_time = current_time;
        printf(".");
        fflush(stdout);
    }
    if (current_time - *last_minute_time >= 60) {
        *last_minute_time = current_time;
        print_current_time(current_time);
    }
}

void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp) {
    printf("DEBUG: combination = \"%s\"\n", combination);
    printf("Combination: %s\n", combination);    
    if (!has_repeats(combination, combination_len, allow_repeats)) {
        // Create new file if necessary
        if (*fp == NULL) {
            snprintf(filename, MAX_FILENAME_LEN, "generated%04llu.txt", *file_count);
            *fp = fopen(filename, "w");
            if (*fp == NULL) {
                fprintf(stderr, "Error: Could not open output file %s.\n", filename);
                exit(1);
            }
            printf("Creating file: %s\n", filename);
            fprintf(list_fp, "%s %s\n", combination, filename);
        }
        // Write combination to file
        fprintf(*fp, "%s\n", combination);
        // Close file and create new one if it's too big
        if (ftell(*fp) >= 1000000000) {  // 1 GB
            fclose(*fp);
            (*file_count)++;
            *fp = NULL;
        }
    }
}
