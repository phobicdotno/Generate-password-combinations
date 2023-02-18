#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NUM_CHARS 62  // Number of possible characters
#define MAX_FILENAME_LEN 100  // Maximum length of filename

// Function prototypes
//void pad_words(int num_words, int* word_lengths, char** words, int max_word_length);
int has_repeats(char* combination, int combination_len, int allow_repeats);
void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp);
void print_progress_and_time(time_t current_time, time_t* last_tick_time, time_t* last_minute_time);
void print_current_time(time_t current_time);
void free_memory(int num_words, int* word_lengths, char** words);
void generate_words(int* num_words, int** word_lengths, char*** words, FILE* wordlist_fp);
void generate_combinations(int min_len, int max_len, int allow_special_chars, int allow_repeats, char *wordlist_file);

// Main function
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


// Generate combinations of characters
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
    if (wordlist_file != NULL) {
        FILE* wordlist_fp = fopen(wordlist_file, "r");
        if (wordlist_fp != NULL) {
            generate_words(&num_words, &word_lengths, &words, wordlist_fp);
            fclose(wordlist_fp);
        } else {
            printf("Failed to open wordlist file '%s'.\n", wordlist_file);
            return;
        }
    } else {
        printf("Wordlist file is required.\n");
        return;
    }

    // Generate combinations using words from wordlist
    for (combination_len = min_len; combination_len <= max_len; combination_len++) {
        unsigned long long num_combinations = pow(num_words, combination_len);
        for (unsigned long long i = 0; i < num_combinations; i++) {
            memset(combination, 0, max_len + 1);
            unsigned long long j = i;
            int k;
            for (k = 0; k < combination_len; k++) {
                int index = j % num_words;
                strcat(combination, words[index]);
                j /= num_words;
            }
            int has_repeating_chars = has_repeats(combination, strlen(combination), allow_repeats);
            if (!has_repeating_chars) {
                write_combination(combination, strlen(combination), allow_repeats, allow_special_chars, &fp, filename, &file_count, list_fp);
            }
        }
        // Print progress and time every 10 seconds, and print current time every minute
        time(&current_time);
        print_progress_and_time(current_time, &last_tick_time, &last_minute_time);
    }
    free_memory(num_words, word_lengths, words);
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

// Write combination of characters to file
void write_combination(char* combination, int combination_len, int allow_repeats, int allow_special_chars, FILE** fp, char* filename, unsigned long long* file_count, FILE* list_fp) {
    if (!has_repeats(combination, combination_len, allow_repeats)) {
        // Create new file if necessary
        if (*fp == NULL) {
            snprintf(filename, MAX_FILENAME_LEN, "generated%04llu.txt", *file_count);
            *fp = fopen(filename, "w");
            printf("Creating file: %s\n", filename);
            fprintf(list_fp, "%s %s\n", combination, filename);
        }
        // Write combination to file
        fprintf(*fp, "%s\n", combination);
        // Close file and create new one if it's too big
        if (ftell(*fp) >= 1000000000) {  // 1 GB
            fclose(*fp);
            *fp = NULL;
            (*file_count)++;
        }
    }
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

// Print current time
void print_current_time(time_t current_time) {
    char* c = ctime(&current_time);
    printf("\nCurrent time: %s", c);
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
