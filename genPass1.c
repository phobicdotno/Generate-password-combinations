#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generateCombinations(char* buffer, int bufferSize, int minLength, int maxLength) {
  if (bufferSize == maxLength) {
    printf("%s\n", buffer);
    return;
  }
  for (char c = 'A'; c <= 'z'; c++) {
    buffer[bufferSize] = c;
    generateCombinations(buffer, bufferSize + 1, minLength, maxLength);
  }
}

int main() {
  int minLength, maxLength;
  size_t maxFileSize;

  printf("Enter the minimum length: ");
  scanf("%d", &minLength);
  printf("Enter the maximum length: ");
  scanf("%d", &maxLength);
  printf("Enter the max file size in MB: ");
  scanf("%zd", &maxFileSize);

  maxFileSize *= 1024 * 1024;

  for (int i = minLength; i <= maxLength; i++) {
    char* buffer = malloc(i + 1);
    memset(buffer, 0, i + 1);
    generateCombinations(buffer, 0, minLength, maxLength);
    free(buffer);
  }

  return 0;
}
