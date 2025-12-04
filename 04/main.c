#include <stdio.h>
#include <string.h>

// 135 lines with 136 characters max (one of which is \n)
#define MAX_FILESIZE 18360

int main()
{
    FILE* inputFile = fopen("input", "r");
    if (!inputFile) {
        fprintf(stderr, "Couldn't read input file\n");
        return 1;
    }
    char input[MAX_FILESIZE + 1];
    fread(input, 1, MAX_FILESIZE, inputFile);
    int i, neighbours, counter = 0;
    int inputLength = (int)strlen(input);
    int lineLength = (int)(strchr(input, '\n') - input) + 1;
    //
    // Part One
    //

    for (i = 0; i < inputLength; i++) {
        if (input[i] != '@') continue;
        neighbours = 0;

        // Order: starting top left clockwise
        if (i - lineLength - 1 >= 0 && input[i - lineLength - 1] == '@') neighbours++;
        if (i - lineLength >= 0 && input[i - lineLength] == '@') neighbours++;
        if (i - lineLength + 1 >= 0 && input[i - lineLength + 1] == '@') neighbours++;
        if (input[i + 1] == '@') neighbours++;
        if (i + lineLength + 1 < inputLength && input[i + lineLength + 1] == '@') neighbours++;
        if (i + lineLength < inputLength && input[i + lineLength] == '@') neighbours++;
        if (i + lineLength - 1 < inputLength && input[i + lineLength - 1] == '@') neighbours++;
        if (i - 1 >= 0 && input[i - 1] == '@') neighbours++;

        if (neighbours < 4) counter++;
    }
    printf("Part one: %d\n", counter);

    //
    // Part two
    //

    // Using true false in C without stdbool feels wrong but is valid C23
    int removeAgain = true;
    neighbours = 0;
    counter = 0;

    while (removeAgain) {
        removeAgain = false;
        for (i = 0; i < inputLength; i++) {
            if (input[i] != '@') continue;
            neighbours = 0;

            // Order: starting top left clockwise
            if (i - lineLength - 1 >= 0 && input[i - lineLength - 1] == '@') neighbours++;
            if (i - lineLength >= 0 && input[i - lineLength] == '@') neighbours++;
            if (i - lineLength + 1 >= 0 && input[i - lineLength + 1] == '@') neighbours++;
            if (input[i + 1] == '@') neighbours++;
            if (i + lineLength + 1 < inputLength && input[i + lineLength + 1] == '@') neighbours++;
            if (i + lineLength < inputLength && input[i + lineLength] == '@') neighbours++;
            if (i + lineLength - 1 < inputLength && input[i + lineLength - 1] == '@') neighbours++;
            if (i - 1 >= 0 && input[i - 1] == '@') neighbours++;

            if (neighbours < 4) {
                removeAgain = true;
                input[i] = '.';
                counter++;
            }
        }
    }
    printf("Part two: %d\n", counter);
}
