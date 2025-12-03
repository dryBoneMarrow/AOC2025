#include <stdio.h>
#include <string.h>

#define numJolts 12

long long int pow10int(int a)
{
    long long int b = 1;
    for (int i = 0; i < a; i++) {
        b *= 10;
    }
    return b;
}

int main()
{
    FILE* input = fopen("input", "r");
    char currLine[102] = {}; // 100 numbers + newline + \0
    int i, greatestNum, greatestNumPos, currNum;
    unsigned long long result = 0;
    //
    // Part One
    //

    while (fgets(currLine, 102, input)) {
        greatestNum = currLine[0] - '0';
        greatestNumPos = 0;
        for (i = 1; i < (int)strlen(currLine) - 2 && greatestNum != 9;
            i++) { // -2: -1 because newline, -1 because last one can't be used
            currNum = currLine[i] - '0';
            if (currNum > greatestNum) {
                greatestNum = currNum;
                greatestNumPos = i;
            }
        }
        result += greatestNum * 10;
        greatestNum = currLine[greatestNumPos + 1] - '0';

        for (i = greatestNumPos + 2; i < (int)strlen(currLine) - 1 && greatestNum != 9;
            i++) //+2: +1 because greatesNum already taken, +1 because first one already greatestNum
                 // above
        {
            currNum = currLine[i] - '0';
            if (currNum > greatestNum) {
                greatestNum = currNum;
            }
        }
        result += greatestNum;
    }
    printf("Part one: %llu\n", result);

    //
    // Part two
    //

    rewind(input);
    int j;
    result = 0;

    while (fgets(currLine, 102, input)) {
        greatestNum = currLine[0] - '0';
        greatestNumPos = 0;
        for (i = numJolts - 1; i >= 0; i--) {
            greatestNum = currLine[greatestNumPos] - '0';
            for (j = greatestNumPos + 1; j < (int)strlen(currLine) - 1 - i && greatestNum != 9;
                j++) { // -2: -1 because newline, -i because i one can't be used
                currNum = currLine[j] - '0';
                if (currNum > greatestNum) {
                    greatestNum = currNum;
                    greatestNumPos = j;
                }
            }
            result += greatestNum * pow10int(i);
            greatestNumPos++;
            // printf("Jolt Nr. %d: %d\n", numJolts - i, greatestNum);
        }
    }
    printf("Part two: %llu\n", result);
}
