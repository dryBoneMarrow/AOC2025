#include <stdio.h>
#include <string.h>

typedef struct {
    const char* position;
    long long unsigned numPaths;
} pathOracle_t;

void traverseTimelines(const char* start, const int lineLength, const char* end,
    unsigned long long* counter, pathOracle_t* knownPaths, int* numKnownPaths)
{
    int i, j;
    unsigned long long localCounter = 0;
    for (i = 1; start + i * lineLength < end; i++) {
        if (start[i * lineLength] == '^') {
            // Use cached data if available
            for (j = 0; j < *numKnownPaths; j++) {
                if (knownPaths[j].position == start + i * lineLength) {
                    localCounter = knownPaths[j].numPaths;
                    goto out;
                }
            }
            localCounter = 1;
            traverseTimelines(start + i * lineLength - 1, lineLength, end, &localCounter,
                knownPaths, numKnownPaths);
            traverseTimelines(start + i * lineLength + 1, lineLength, end, &localCounter,
                knownPaths, numKnownPaths);
            // Cache data for next time
            knownPaths[*numKnownPaths].numPaths = localCounter;
            knownPaths[*numKnownPaths].position = start + i * lineLength;
            (*numKnownPaths)++;
            break;
        }
    }
out:
    (*counter) += localCounter;
}

int main()
{
    char input[] = {
#embed "input"
        , '\0'
    };

    //
    // Part one
    //

    int lineLength;
    for (lineLength = 0; input[lineLength] != '\n'; lineLength++)
        ;
    lineLength++;

    int i;
    unsigned long long splits = 0;
    char currChar;
    bool beamPos[lineLength - 1] = {};
    beamPos[lineLength / 2 - 1] = true;

    // numUsedSplitter is for part two
    int numUsedSplitter = 0;
    // Assumes no ^ right next to each other or at the border (is the case both in input and
    // testinput)
    for (i = 0; (currChar = input[i]); i++) {
        if (currChar == '^' && beamPos[i % lineLength]) {
            splits++;
            beamPos[i % lineLength] = false;
            beamPos[i % lineLength + 1] = true;
            beamPos[i % lineLength - 1] = true;
            numUsedSplitter++;
        }
    }
    printf("Part one: %llu\n", splits);

    //
    // Part two
    //
    splits = 1;
    pathOracle_t knownPaths[numUsedSplitter] = {};
    int knownPathCounter = 0;
    traverseTimelines(input + lineLength / 2 - 1, lineLength, strchr(input, '\0'), &splits,
        knownPaths, &knownPathCounter);
    printf("Part two: %llu\n", splits);
    return 0;
}
