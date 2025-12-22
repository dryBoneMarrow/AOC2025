#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long x;
    long y;
} coord_t;

typedef struct {
    long position;
    long from;
    long to;
} line_t;

unsigned long long absllu(long a)
{
    if (a < 0) return -a;
    return a;
}

static int compLine(const void* line1, const void* line2)
{
    if (((line_t*)line1)->position < ((line_t*)line2)->position) return -1;
    if (((line_t*)line1)->position > ((line_t*)line2)->position) return 1;
    return 0;
}

int overlap(int amin, int amax, int bmin, int bmax)
{
    if (amin <= bmax && bmin <= amax) return 1;
    return 0;
}
int main()
{
    char input[] = {
#embed "testinput"
        , '\0'
    };

    // Parse input
    unsigned i, j, numCoords = 0;
    for (i = 0; input[i]; i++) {
        if (input[i] == '\n') numCoords++;
    }

    coord_t coords[numCoords] = {};
    coord_t* coordP = coords;
    for (i = 0; input[i]; i++) {
        for (; input[i] != ','; i++) {
            coordP->x *= 10;
            coordP->x += input[i] - '0';
        }
        for (i++; input[i] != '\n'; i++) {
            coordP->y *= 10;
            coordP->y += input[i] - '0';
        }
        coordP++;
    }

    //
    // Part one
    //

    // Get largest area
    long long unsigned result = 0, area;
    for (i = 0; i < numCoords; i++) {
        for (j = i + 1; j < numCoords; j++) {
            area
                = (absllu(coords[i].x - coords[j].x) + 1) * (absllu(coords[i].y - coords[j].y) + 1);
            if (result < area) result = area;
        }
    }
    printf("Part one: %llu\n", result);

    //
    // Part two
    //

    //// Make, fill and sort lists with all horizontal and vertical lines
    // Verified that every red tiles are indeed always corners
    line_t horLines[numCoords / 2];
    line_t* horLineP = horLines;
    line_t* relHorLineStart;
    line_t* relHorLineEnd;
    line_t verLines[numCoords / 2];
    line_t* verLineP = verLines;
    line_t* relVerLineStart;
    line_t* relVerLineEnd;
    for (i = 1; i < numCoords; i++) {
        if (coords[i].x == coords[i - 1].x) {
            verLineP->position = coords[i].x;
            verLineP->from = (coords[i].y < coords[i - 1].y) ? coords[i].y : coords[i - 1].y;
            verLineP->to = (coords[i].y < coords[i - 1].y) ? coords[i - 1].y : coords[i].y;
            verLineP++;
        }
        if (coords[i].y == coords[i - 1].y) {
            horLineP->position = coords[i].y;
            horLineP->from = (coords[i].x < coords[i - 1].x) ? coords[i].x : coords[i - 1].x;
            horLineP->to = (coords[i].x < coords[i - 1].x) ? coords[i - 1].x : coords[i].x;
            horLineP++;
        }
    }
    // Line between last and first coordinates
    if (coords[numCoords - 1].x == coords[0].x) {
        verLineP->position = coords[0].x;
        verLineP->from = (coords[i - 1].y < coords[0].y) ? coords[i - 1].y : coords[0].y;
        verLineP->to = (coords[i - 1].y < coords[0].y) ? coords[0].y : coords[i - 1].y;
    }
    if (coords[numCoords - 1].y == coords[0].y) {
        horLineP->position = coords[0].y;
        horLineP->from = (coords[i - 1].x < coords[0].x) ? coords[i - 1].x : coords[0].x;
        horLineP->to = (coords[i - 1].x < coords[0].x) ? coords[0].x : coords[i - 1].x;
    }

    qsort(horLines, numCoords / 2, sizeof(line_t), compLine);
    qsort(verLines, numCoords / 2, sizeof(line_t), compLine);

    // Find largest area that is inside
    result = 0;
    // t=top, b=bottom, l=left, r=right
    long xmin, xmax, ymin, ymax;
    for (i = 0; i < numCoords; i++) {
        for (j = i + 1; j < numCoords; j++) {
            // Very high chance that a 1*X or X*1 rectangle isn't the biggest and like this we get
            // rid of potential edge-case
            if (coords[i].x == coords[j].x || coords[i].y == coords[j].y) continue;
            area
                = (absllu(coords[i].x - coords[j].x) + 1) * (absllu(coords[i].y - coords[j].y) + 1);
            if (result < area) {
                relHorLineStart = horLines;
                relVerLineStart = verLines;

                // Assumes that there are no parallel lines next to each other (is the case for
                // testinput, unknown for input)

                // i**
                // **j
                if (coords[i].y <= coords[j].y && coords[i].x <= coords[j].x) {
                    ymin = coords[i].y;
                    xmin = coords[i].x;
                    ymax = coords[j].y;
                    xmax = coords[j].x;
                }
                // j**
                // **i
                else if (coords[i].y >= coords[j].y && coords[i].x >= coords[j].x) {
                    ymin = coords[j].y;
                    xmin = coords[j].x;
                    ymax = coords[i].y;
                    xmax = coords[i].x;
                }
                // **j
                // i**
                else if (coords[i].y >= coords[j].y && coords[i].x <= coords[j].x) {
                    ymin = coords[j].y;
                    xmin = coords[i].x;
                    ymax = coords[i].y;
                    xmax = coords[j].x;
                }
                // **i
                // j**
                else if (coords[j].y >= coords[i].y && coords[j].x <= coords[i].x) {
                    ymin = coords[i].y;
                    xmin = coords[j].x;
                    ymax = coords[j].y;
                    xmax = coords[i].x;
                }
                // TODO: Check if already at border and if so what happens
                while (relHorLineStart->position <= ymin) {
                    relHorLineStart++;
                }
                relHorLineEnd = relHorLineStart;
                while (relHorLineEnd->position < ymax) {
                    relHorLineEnd++;
                }
                relHorLineEnd--;
                while (relVerLineStart->position <= xmin) {
                    relVerLineStart++;
                }
                relVerLineEnd = relVerLineStart;
                while (relVerLineEnd->position < xmax) {
                    relVerLineEnd++;
                }
                relVerLineEnd--;
                horLineP = relHorLineStart;
                while (horLineP != relHorLineEnd) {
                    // printf("%ld: %ld-%ld\n", horLineP->position, horLineP->from, horLineP->to);
                    if (overlap(horLineP->from, horLineP->to, xmin, xmax)) {
                        goto nextRectangle;
                    }
                    horLineP++;
                }

                verLineP = relVerLineStart;
                while (verLineP != relVerLineEnd) {
                    if (overlap(verLineP->from, verLineP->to, ymin, ymax)) {
                        goto nextRectangle;
                    }
                    verLineP++;
                }
                result = area;
                // printf("(%ld,%ld)-(%ld,%ld)\n", xmin, ymin, xmax, ymax);
            }
        nextRectangle:
        }
    }
    printf("Part two: %llu\n", result);
}
