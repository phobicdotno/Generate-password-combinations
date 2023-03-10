#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read lines from a file and store them in an array
char** read_lines_from_file(const char* filename, int* num_lines) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // Count the number of lines in the file
    char c;
    int count = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            count++;
        }
    }
    rewind(fp);

    // Allocate memory for the array of lines
    char** lines = (char**) malloc(count * sizeof(char*));
    if (lines == NULL) {
        printf("Error allocating memory\n");
        return NULL;
    }

    // Allocate memory for the buffer for each line
    size_t max_line_len = 100;
    for (int i = 0; i < count; i++) {
        lines[i] = (char*) malloc(max_line_len * sizeof(char));
        if (lines[i] == NULL) {
            printf("Error allocating memory\n");
            // Free memory allocated so far
            for (int j = 0; j < i; j++) {
                free(lines[j]);
            }
            free(lines);
            return NULL;
        }
    }

    // Read the lines from the file
    int i = 0;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&lines[i], &len, fp)) != -1) {
        if (read == 1 && lines[i][0] == '\n') {
            // Don't remove the newline character for empty lines
            lines[i][0] = '\0';
        } else {
            // Remove the newline character, if present
            if (lines[i][read-1] == '\n') {
                lines[i][read-1] = '\0';
            }
        }
        i++;
    }

    if (ferror(fp)) {
        printf("Error reading file\n");
        // Free memory allocated so far
        for (int j = 0; j < i; j++) {
            free(lines[j]);
        }
        free(lines);
        return NULL;
    }

    *num_lines = count;
    fclose(fp);
    return lines;
}

// Function to print a combination to a file
void print_combination(FILE* file, char** lines, int* indices, int num_indices) {
    for (int i = 0; i < num_indices; i++) {
        fprintf(file, "%s", lines[indices[i]]);
    }
    fprintf(file, "\n");
}

void generate_combinations(FILE* file, char** lines, int num_lines, int min_len, int max_len) {
    int* indices = (int*) malloc(max_len * sizeof(int));
    if (indices == NULL) {
        printf("Error allocating memory\n");
        return;
    }

    // Find the index of the special line
    int special_line_index = -1;
    for (int i = 0; i < num_lines; i++) {
        if (strlen(lines[i]) > 1) {
            special_line_index = i;
            break;
        }
    }

    if (special_line_index == -1) {
        printf("Error: no line with length > 1\n");
        free(indices);
        return;
    }

    // Initialize the indices to the first max_len lines
    for (int i = 0; i < max_len; i++) {
        indices[i] = i;
    }

    int i = max_len - 1;
    while (i >= 0) {
        // Print the current combination if it meets the length criteria
        int len = 0;
        int has_special_line = 0;
        int num_special_lines = 0;
        for (int j = 0; j <= i; j++) {
            len += strlen(lines[indices[j]]);
            if (indices[j] == special_line_index) {
                has_special_line = 1;
                num_special_lines++;
            }
        }
        if (len >= min_len && len <= max_len && has_special_line && num_special_lines == 1) {
            print_combination(file, lines, indices, i+1);
        }

        // Move to the next combination
        int found_next = 0;
        while (!found_next) {
            indices[i]++;
            if (indices[i] <= num_lines - (max_len - i)) {
                found_next = 1;
            } else {
                i--;
                if (i < 0) {
                    break;
                }
            }
        }

        if (i < max_len - 1) {
            indices[i+1] = indices[i] + 1;
            i++;
        }
    }

    free(indices);
}

int main() {
    char filename[] = "list2.txt";
    int num_lines;
    char** lines = read_lines_from_file(filename, &num_lines);
    if (lines == NULL) {
        return 1;
    }

    int min_len, max_len;
    printf("Enter the minimum length of combinations: ");
    scanf("%d", &min_len);
    printf("Enter the maximum length of combinations: ");
    scanf("%d", &max_len);

    FILE* file = fopen("combinations.txt", "w");
    if (file == NULL) {
        printf("Error opening output file\n");
        return 1;
    }

    generate_combinations(file, lines, num_lines, min_len, max_len);

    fclose(file);

    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}
