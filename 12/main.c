#include <stdio.h>

// Is the same in testinput and input, so probably same for everyone
#define NUM_SHAPES 6

// Implemented as macro because copied from main function and too lazy to rewrite using parameters
// Can be used in combination with the diff shell tool to «proof» correctness for given input file
#define testParsing()                                                                              \
    for (i = 0; i < NUM_SHAPES; i++) {                                                             \
        printf("%d:\n%c%c%c\n%c%c%c\n%c%c%c\n\n", i, (presents[i].shape[0]) ? '#' : '.',           \
            (presents[i].shape[1]) ? '#' : '.', (presents[i].shape[2]) ? '#' : '.',                \
            (presents[i].shape[3]) ? '#' : '.', (presents[i].shape[4]) ? '#' : '.',                \
            (presents[i].shape[5]) ? '#' : '.', (presents[i].shape[6]) ? '#' : '.',                \
            (presents[i].shape[7]) ? '#' : '.', (presents[i].shape[8]) ? '#' : '.');               \
    }                                                                                              \
    for (i = 0; i < numTrees; i++) {                                                               \
        printf("%hdx%hd: %hd %hd %hd %hd %hd %hd\n", trees[i].x, trees[i].y,                       \
            trees[i].numPresents[0], trees[i].numPresents[1], trees[i].numPresents[2],             \
            trees[i].numPresents[3], trees[i].numPresents[4], trees[i].numPresents[5]);            \
    }

typedef struct {
    bool shape[9];
    short surfaceArea;
} present_t;

typedef struct {
    short x;
    short y;
    short numPresents[NUM_SHAPES];
    short totalPresents;
    bool ruledOut;
    bool ruledIn;
} region_t;

int main()
{
    char input[] = {
#ifdef testinput
#embed "testinput"
#else
#embed "input"
#endif
        , '\0'
    };
    int i, j, k, numTrees = 0;

    for (i = 0; input[i]; i++) {
        if (input[i] == '\n') {
            numTrees = 0;
            continue;
        }
        while (input[i] != '\n') {
            i++;
        }
        numTrees++;
    }

    region_t trees[numTrees] = {};
    region_t* treeP = trees;
    present_t presents[NUM_SHAPES] = {};

    for (i = 0, j = 0; j < NUM_SHAPES; j++) {
        while (input[i] != '\n')
            i++;
        i++;
        for (k = 0; k < 9;) {
            for (int _ = 0; _ < 3; _++, k++, i++) {
                if (input[i] == '#') {
                    presents[j].shape[k] = true;
                } else if (input[i] == '.') {
                    presents[j].shape[k] = false;
                } else {
                    puts("Gopfertelli nomale");
                }
            }
            i++;
        }
        i++;
    }

    for (; input[i]; i++) {
        // Why testinput one digit dimension but real input doesn't :(
        while (input[i] != 'x') {
            treeP->x *= 10;
            treeP->x += input[i] - '0';
            i++;
        }
        i++;
        while (input[i] != ':') {
            treeP->y *= 10;
            treeP->y += input[i] - '0';
            i++;
        }
        i++;
        // Why testinput one digit present number but real input doesn't :( :(
        for (j = 0; j < NUM_SHAPES; j++) {
            i++;
            while (input[i] != ' ' && input[i] != '\n') {
                treeP->numPresents[j] *= 10;
                treeP->numPresents[j] += input[i] - '0';
                i++;
            }
        }
        treeP++;
    }

    for (i = 0; i < NUM_SHAPES; i++) {
        for (j = 0; j < 9; j++) {
            if (presents[i].shape[j]) presents[i].surfaceArea++;
        }
    }

    for (i = 0; i < numTrees; i++) {
        for (j = 0; j < NUM_SHAPES; j++)
            trees[i].totalPresents += trees[i].numPresents[j];
    }

    //
    // Part one
    //

    // Kind of a joke (a trick question)
    // In the real input, every area under the tree is either to small to fit the presents even if
    // we could use a scissor (thus if they could be arranged perfectly without gaps) or it is big
    // enough so that even if we assume each present to be a 3x3 square (every oddly-shaped present
    // fits into such a square) there's enough space making the problem disappointingly trivial.

    int minAreaRequired;
    for (i = 0; i < numTrees; i++) {
        // Check if enough area under tree for when shapes could be combined perfectly (if not 0%
        // possible)
        minAreaRequired = 0;
        for (j = 0; j < NUM_SHAPES; j++) {
            minAreaRequired += trees[i].numPresents[j] * presents[j].surfaceArea;
        }
        if (minAreaRequired > trees[i].x * trees[i].y) {
            // printf("Nr. %3d has been ruled out.\n", i);
            trees[i].ruledOut = true;
            continue;
        }

        // Check if enough area under tree if shapes are 3x3 squares (if yes then 100% possible)
        if ((trees[i].x / 3) * (trees[i].y / 3) >= trees[i].totalPresents) {
            // printf("Nr. %3d has been ruled in.\n", i);
            trees[i].ruledIn = true;
            continue;
        }
    }
    int nRuledIn = 0;
    int nRuledOut = 0;
    int nUnknown = 0;
    for (i = 0; i < numTrees; i++) {
        if (trees[i].ruledIn)
            nRuledIn++;
        else if (trees[i].ruledOut)
            nRuledOut++;
        else
            nUnknown++;
    }
    printf("Ruled in:\t %d (%f%%)\n"
           "Ruled out:\t %d (%f%%)\n"
           "Unknown:\t %d (%f%%)\n",
        nRuledIn, (double)nRuledIn / numTrees * 100, nRuledOut, (double)nRuledOut / numTrees * 100,
        nUnknown, (double)nUnknown / numTrees * 100);
    if (nUnknown) {
        printf("Part one: between %d and %d (for my real input there's no range, thus no further "
               "reducing of unkowns)\n",
            nRuledIn, nRuledIn + nUnknown);
    }
    if (!nUnknown) {
        printf("Part one: %d\n", nRuledIn);
    }

    //
    // Part two
    //
    printf("Part two: 42\n");
}
