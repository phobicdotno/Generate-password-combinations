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
#include <sys/stat.h>

#define MAX_COMBINATION_LENGTH 20
#define MBPerFile 100
#define MB_TO_BYTES(x) (x * 1024 * 1024)
#define MAX_FILE_SIZE MB_TO_BYTES(MBPerFile)

void generateCombinations(int length, char *combination, int currentLength, FILE *fp) {
  if (length == currentLength) {
    fprintf(fp, "%s\n", combination);
    return;
  }

  for (int i = 33; i <= 126; i++) {
    combination[currentLength] = (char)i;
    generateCombinations(length, combination, currentLength + 1, fp);
  }
}

int main() {
  int minLength, maxLength;
  char combination[MAX_COMBINATION_LENGTH + 1];
  FILE *fp;
  time_t rawtime;
  struct tm * timeinfo;
  char path[100];
  char filename[100];
  int fileCounter = 1;
  int fileSize = 0;

  printf("Enter the minimum length: ");
  scanf("%d", &minLength);

  printf("Enter the maximum length: ");
  scanf("%d", &maxLength);

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(path, sizeof(path), "./%Y-%m-%d-%H-%M-%S", timeinfo);
  mkdir(path, 0700);

  for (int i = minLength; i <= maxLength; i++) {
    memset(combination, 0, sizeof(combination));
    sprintf(filename, "%s/combinations-%d.txt", path, fileCounter);
    fp = fopen(filename, "w");
    generateCombinations(i, combination, 0, fp);
    fileSize = ftell(fp);
    if (fileSize >= MAX_FILE_SIZE) {
      fileCounter++;
      fclose(fp);
      sprintf(filename, "%s/combinations-%d.txt", path, fileCounter);
      fp = fopen(filename, "w");
      fileSize = 0;
    }
  }
  fclose(fp);
  return 0;
}
