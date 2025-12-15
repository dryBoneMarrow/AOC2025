// Uses uniond-find data structure, implementation loosely inspired by
// https://github.com/luistar/union-find-c/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Number of connections necessary in part one
#define CONN_P1 1000

typedef struct circuit {
    struct circuit* parent;
    int size;
    struct {
        long x;
        long y;
        long z;
    } coords;
} circuit_t;

typedef struct {
    circuit_t* a;
    circuit_t* b;
    long unsigned distance;
} distance_t;

long unsigned getDistance(const circuit_t* a, const circuit_t* b)
{
    // sqrt is missing but doesn't matter and no math.h like this
    return (a->coords.x - b->coords.x) * (a->coords.x - b->coords.x)
        + (a->coords.y - b->coords.y) * (a->coords.y - b->coords.y)
        + (a->coords.z - b->coords.z) * (a->coords.z - b->coords.z);
}

static int compDist(const void* dist1, const void* dist2)
{
    if (((distance_t*)dist1)->distance < ((distance_t*)dist2)->distance) return -1;
    if (((distance_t*)dist1)->distance > ((distance_t*)dist2)->distance) return 1;
    return 0;
}

circuit_t* getRepresentative(circuit_t* a)
{
    circuit_t* origCirc = a;
    while (a->parent) {
        a = a->parent;
    }
    if (origCirc != a) origCirc->parent = a;
    return a;
}

int mergeCircuits(circuit_t* a, circuit_t* b)
{
    circuit_t* repA = getRepresentative(a);
    circuit_t* repB = getRepresentative(b);
    if (repA != repB) {
        if (repA->size > repB->size) {
            repB->parent = repA;
            repA->size += repB->size;
        } else {
            repA->parent = repB;
            repB->size += repA->size;
        }
        return 1;
    }
    return 0;
}

int main()
{
    char input[] = {
#embed "input"
        , '\0'
    };
    long i, j;
    long long unsigned result = 0;

    // Count coordinates

    int circuitNum = 0;
    for (i = 0; input[i]; i++) {
        if (input[i] == '\n') circuitNum++;
    }

    // Parse input

    circuit_t circuits[circuitNum] = {};
    circuit_t* circuitP = circuits;
    for (i = 0; input[i]; i++) {
        for (; input[i] != ','; i++) {
            circuitP->coords.x *= 10;
            circuitP->coords.x += input[i] - '0';
        }
        for (i++; input[i] != ','; i++) {
            circuitP->coords.y *= 10;
            circuitP->coords.y += input[i] - '0';
        }
        for (i++; input[i] != '\n'; i++) {
            circuitP->coords.z *= 10;
            circuitP->coords.z += input[i] - '0';
        }
        circuitP->size = 1;
        circuitP->parent = NULL;
        circuitP++;
    }

    // Calculate distances

    // Binomial(x,2)
    long numCombinations = circuitNum * (circuitNum - 1) / 2;
    distance_t* distances = calloc(numCombinations, sizeof(distance_t));
    distance_t* distancesP = distances;
    for (i = 0; i < circuitNum; i++) {
        for (j = i + 1; j < circuitNum; j++) {
            distancesP->a = circuits + i;
            distancesP->b = circuits + j;
            distancesP->distance = getDistance(distancesP->a, distancesP->b);
            distancesP++;
        }
    }

    // Sort distances

    qsort(distances, numCombinations, sizeof(distance_t), compDist);

    // Part one
    circuit_t *largestCircs[3] = { distances[0].a, distances[1].a, distances[2].a }, *currRep,
              *tempP;
    for (i = 0; i < CONN_P1; i++) {
        mergeCircuits(distances[i].a, distances[i].b);
    }
    for (i = 0; i < CONN_P1; i++) {
    continueOuter:
        currRep = getRepresentative(distances[i].a);
        for (j = 0; j < 3; j++) {
            if (currRep == largestCircs[j]) {
                i++;
                goto continueOuter;
            }
        }
        if (largestCircs[2]->size >= currRep->size) continue;
        largestCircs[2] = currRep;
        for (j = 2; j > 0; j--) {
            if (largestCircs[j] > largestCircs[j - 1]) {
                tempP = largestCircs[j];
                largestCircs[j] = largestCircs[j - 1];
                largestCircs[j - 1] = tempP;
            }
        }
    }
    result = largestCircs[0]->size * largestCircs[1]->size * largestCircs[2]->size;
    printf("Part one: %llu\n", result);

    // Part two
    long currX1, currX2;
    for (; i < numCombinations; i++) {
        if (mergeCircuits(distances[i].a, distances[i].b)) {
            currX1 = distances[i].a->coords.x;
            currX2 = distances[i].b->coords.x;
        }
    }

    result = currX1 * currX2;
    printf("Part two: %llu\n", result);
    free(distances);
}
