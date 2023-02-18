#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FILENAME_LEN 100  // Maximum length of filename

void generate_combinations(int min_len, int max_len, char** objects, int num_objects);

void generate_words(int min_len, int max_len, char** words, int num_words, char** objects, int num_objects);

void print_combinations(int combination_len, char** objects, int num_objects, char* prefix);

int main() {
    char filename[MAX_FILENAME_LEN];
    printf("Enter name of file to import objects: ");
    scanf("%s", filename);

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file: %s\n", filename);
        return 1;
    }

    int num_objects = 0;
    char** objects = (char**) malloc(1000 * sizeof(char*));
    char line[256];
    while (fgets(line, 256, fp) != NULL) {
        line[strcspn(line, "\n")] = 0;
        objects[num_objects] = (char*) malloc((strlen(line) + 1) * sizeof(char));
        strcpy(objects[num_objects], line);
        num_objects++;
    }
    fclose(fp);

    int min_len, max_len;
    printf("Enter minimum length of combinations: ");
    scanf("%d", &min_len);
    printf("Enter maximum length of combinations: ");
    scanf("%d", &max_len);

    generate_combinations(min_len, max_len, objects, num_objects);

    // Free memory allocated for objects
    for (int i = 0; i < num_objects; i++) {
        free(objects[i]);
    }
    free(objects);

    return 0;
}

void generate_combinations(int min_len, int max_len, char** objects, int num_objects) {
    for (int i = min_len; i <= max_len; i++) {
        print_combinations(i, objects, num_objects, "");
    }
}

void generate_words(int min_len, int max_len, char** words, int num_words, char** objects, int num_objects) {
    for (int i = 0; i < num_words; i++) {
        for (int j = 0; j < num_objects; j++) {
            if (strlen(words[i]) + strlen(objects[j]) >= min_len && strlen(words[i]) + strlen(objects[j]) <= max_len) {
                char* combination = (char*) malloc((strlen(words[i]) + strlen(objects[j]) + 1) * sizeof(char));
                strcpy(combination, words[i]);
                strcat(combination, objects[j]);
                print_combinations(max_len - strlen(combination), objects, num_objects, combination);
                free(combination);
            }
        }
    }
}

void print_combinations(int combination_len, char** objects, int num_objects, char* prefix) {
    if (combination_len == 0) {
        printf("%s\n", prefix);
        return;
    }

    for (int i = 0; i < num_objects; i++) {
        char* new_prefix = (char*) malloc((strlen(prefix) + strlen(objects[i]) + 1) * sizeof(char));
        strcpy(new_prefix, prefix);
        strcat(new_prefix, objects[i]);
        print_combinations(combination_len - 1, objects, num_objects, new_prefix);
        free(new_prefix);
    }
}
