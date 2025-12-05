#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxRanges 174
#define maxNums 1000

typedef struct {
    long long unsigned min;
    long long unsigned max;
} range_t;

int compRange(const void* a, const void* b)
{
    range_t range1 = *(range_t*)a;
    range_t range2 = *(range_t*)b;

    if (range1.min < range2.min) return -1;
    if (range1.min == range2.min) return 0;
    if (range1.min > range2.min) return 1;
    unreachable();
}

int main()
{
    char input[] = {
#embed "input"
        , '\0'
    };
    char* inputp = input;
    char* border = strstr(input, "\n\n");

    range_t ranges[maxRanges];
    int numRanges = 0;

    long long unsigned availableNums[maxNums];
    int numAvailables = 0;

    int i, j;
    int areFresh = 0;

    //
    // Part One
    //

    while (inputp < border) {
        sscanf(inputp, "%llu-%llu\n", &ranges[numRanges].min, &ranges[numRanges].max);
        inputp = strchr(inputp, '\n') + 1;
        numRanges++;
    }

    inputp += 1;

    while (*inputp) {
        sscanf(inputp, "%llu\n", &availableNums[numAvailables]);
        inputp = strchr(inputp, '\n') + 1;
        numAvailables++;
    }

    for (i = 0; i < numAvailables; i++) {
        for (j = 0; j < numRanges; j++) {
            if (availableNums[i] >= ranges[j].min && availableNums[i] <= ranges[j].max) {
                areFresh++;
                break;
            }
        }
    }

    printf("Part one: %d\n", areFresh);

    //
    // Part two
    //
    qsort(ranges, numRanges, sizeof(range_t), compRange);
    range_t uniqueRanges[numRanges];
    uniqueRanges[0] = ranges[0];
    int numUniqueRanges = 1;
    range_t* currUniqueRange;
    range_t* currRange;
    unsigned long long numValidIDs = 0;

    for (i = 1; i < numRanges; i++) {
        currUniqueRange = &uniqueRanges[numUniqueRanges - 1];
        currRange = &ranges[i];

        if (currUniqueRange->max >= currRange->min) {
            if (currUniqueRange->max >= currRange->max) continue;

            currUniqueRange->max = currRange->max;
        } else {
            uniqueRanges[numUniqueRanges] = *currRange;
            numUniqueRanges++;
        }
    }
    for (i = 0; i < numUniqueRanges; i++) {
        numValidIDs += uniqueRanges[i].max - uniqueRanges[i].min + 1;
    }
    printf("Part two: %llu\n", numValidIDs);
}
