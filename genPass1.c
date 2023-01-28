//code that generates all possible combinations of all 95 printable ASCII characters (32-126) with the last three 
//characters are numbers and accepts four input parameters, minLength, maxLength, MBPerFile and path to specify the range 
//of lengths of the combinations, the maximum size of the file and the path to save the files:

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void generateCombinations(char *prefix, int remaining, int minLength, int maxLength, int MBPerFile, char *path) {
    int i, j, k;
    if (remaining == 0) {
        // Print the combination to the file
        FILE *fp;
        char filename[100];
        sprintf(filename, "%s/%s-%s.txt", path, prefix, prefix);
        fp = fopen(filename, "a");
        fprintf(fp, "%s\n", prefix);
        fclose(fp);
        return;
    }
    for (i = 32; i <= 126; i++) {
        char newPrefix[remaining + 1];
        sprintf(newPrefix, "%s%c", prefix, i);
        if (remaining == 3) {
            for (j = '0'; j <= '9'; j++) {
                for (k = '0'; k <= '9'; k++) {
                    char newPrefix[remaining + 1];
                    sprintf(newPrefix, "%s%c%c%c", prefix, i,j,k);
                    // Print the combination to the file
                    FILE *fp;
                    char filename[100];
                    sprintf(filename, "%s/%s-%s.txt", path, prefix, newPrefix);
                    fp = fopen(filename, "a");
                    fprintf(fp, "%s\n", newPrefix);
                    fclose(fp);
                }
            }
        }else{
            if(remaining >= minLength && remaining <= maxLength){
                generateCombinations(newPrefix, remaining - 1, minLength, maxLength, MBPerFile, path);
            }
        }
    }
}

int main() {
    int minLength, maxLength, MBPerFile;
    char path[100];
    printf("Enter the minimum length: ");
    scanf("%d", &minLength);
    printf("Enter the maximum length: ");
    scanf("%d", &maxLength);
    printf("Enter the max file size in MB: ");
    scanf("%d", &MBPerFile);
    printf("Enter the path to save the files: ");
    scanf("%s", path);
    for(int i=minLength; i<=
    maxLength; i++) {
    char prefix[i + 1];
    memset(prefix, 0, sizeof(prefix));
    generateCombinations(prefix, i, minLength, maxLength, MBPerFile, path);
}
    return 0;
}
