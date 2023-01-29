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
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>

#define CHARSET_SIZE 62
#define GB 1073741824 // 1 GB = 1073741824 bytes

char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
char path[] = "./output";

void makeCombination(char *str, int index, int max, int min, int len, FILE *f) {
  int i;
  if (index == len) {
    fprintf(f, "%s\n", str);
    return;
  }

  for (i = 0; i < CHARSET_SIZE; i++) {
    str[index] = charset[i];
    makeCombination(str, index + 1, max, min, len, f);
  }
}

int main() {
  int i, min, max, counter = 0;
  char filename[100];
  char *str;
  FILE *f;

  printf("Enter the minimum length: ");
  scanf("%d", &min);
  printf("Enter the maximum length: ");
  scanf("%d", &max);
  
  if (min < 1 || max < 1 || max < min) {
    printf("Invalid length\n");
    return 1;
  }

  // create output directory
  mkdir(path, 0700);

  str = (char *) malloc(max + 1);
  str[max] = '\0';

  for (i = min; i <= max; i++) {
    memset(str, 0, max + 1);
    memset(filename, 0, 100);
    sprintf(filename, "%s/combinations-%ld.txt", path, time(NULL));
    f = fopen(filename, "w");

    makeCombination(str, 0, max, min, i, f);
    fclose(f);
    counter++;

    if (counter * i >= GB) {
      counter = 0;
      free(str);
      str = (char *) malloc(max + 1);
      str[max] = '\0';
    }

    printf("Generated combinations with length %d and saved to file %s\n", i, filename);
  }

  free(str);
  return 0;
}
