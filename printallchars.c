/*
This code uses four nested loops to iterate over 
all 91 printable ASCII characters and print them out line by line.
The c array is used to store the current combination of characters,
which is printed out using printf at the end of each iteration of the innermost loop.
The \n character is used to print each combination on a new line.
*/

#include <stdio.h>

int main() {
    int i, j, k;
    char chars[100] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    for (i = 0; i < 91; i++) {
        printf("%c\n", chars[i]);
    }
    return 0;
}
