#include <stdio.h>
#include <stdlib.h>

// If we interpret string as base 26 number (a=0, b=1, ..., z=25) we get the following:
#define YOU 16608
#define OUT 10003
#define SVR 12731
#define FFT 3529
#define DAC 2030

#define nameToNum(str) (26 * 26 * (*(str) - 'a') + 26 * (*((str) + 1) - 'a') + (*((str) + 2) - 'a'))

typedef struct device_t {
    // Serial is device name interpreted as base 26 number (a=0, b=1, ..., z=25)
    int serial;
    int nOutputs;
    struct device_t** Outputs;
    int nInputs;
    // Don't need them
#if 0
    struct device_t** Inputs;
#endif
    long long unsigned outPaths;
    long unsigned long outPathsDac;
    unsigned long long outPathsFft;
    long long unsigned outPathsDacFft;
} device_t;

static int cmpDevDev(const void* x, const void* y)
{
    if (((device_t*)x)->serial > ((device_t*)y)->serial) return 1;
    if (((device_t*)x)->serial < ((device_t*)y)->serial) return -1;
    return 0;
}

static int cmpIntDev(const void* x, const void* y)
{
    if (*(int*)x > ((device_t*)y)->serial) return 1;
    if (*(int*)x < ((device_t*)y)->serial) return -1;
    return 0;
}

void numToName(int num, char* str)
{
    str[0] = num / 26 / 26 + 'a';
    num %= 26 * 26;
    str[1] = num / 26 + 'a';
    num %= 26;
    str[2] = num + 'a';
    str[3] = '\0';
}

void findPaths1(const device_t* const startNode, const device_t* const endNode,
    unsigned long long* const counter)
{
    // Naïve implementation (for now)
    // (ETA: I probably won't because execution time already satisfyingly fast enough (real 4ms with
    // -Ofast))
    // (EETA: Maybe I will because it's kind of unsatisfying to not at least use a cache)
    // (EEETA: Added buffer (and thus a satisfying solution) in part two, thus wontfix)
    for (int i = 0; i < startNode->nOutputs; i++) {
        findPaths1(startNode->Outputs[i], endNode, counter);
    }
    if (startNode->serial == endNode->serial) (*counter)++;
}

int main()
{
    unsigned long long result = 0;
    //
    // parse input
    //
    const char input[] = {
    // testinput only compatible with part 1 (because missing svr, fft and dac)
    // testinput2 only compatible with part two (because missing you)
    // It's stupid, I know (but feels wrong to modify the test cases)
#embed "input"
        , '\0'
    };
    int numDev = 0, i, j;
    for (i = 0; input[i]; i++) {
        if (input[i] == '\n') numDev++;
    }
    // +1 is the OUT device which is added manually
    numDev++;
    device_t devices[numDev] = {};
    devices[0].serial = OUT;

    device_t* deviceP = devices + 1;
    bool newDev = true;
    for (i = 0; input[i]; i++) {
        if (newDev) {
            deviceP->serial = nameToNum(input + i);
            i += 3;
            newDev = false;
            deviceP++;
        } else if (input[i] == '\n') {
            newDev = true;
        }
    }
    qsort(devices, numDev, sizeof(*devices), &cmpDevDev);

    int someDevice = 0;
    device_t* currOutput;
    for (i = 0; input[i]; i++) {
        someDevice = nameToNum(input + i);
        deviceP = bsearch(&someDevice, devices, numDev, sizeof(*devices), &cmpIntDev);
        i += 4;

        while (input[i] != '\n') {
            i++;

            // Get current output device
            someDevice = nameToNum(input + i);
            currOutput = bsearch(&someDevice, devices, numDev, sizeof(*devices), &cmpIntDev);
            // Handle Output of current device
            deviceP->nOutputs++;
            deviceP->Outputs = realloc(deviceP->Outputs, sizeof(*devices) * deviceP->nOutputs);
            deviceP->Outputs[deviceP->nOutputs - 1] = currOutput;

            // Handle input from current device
            currOutput->nInputs++;
            // Actually we don't need the inputs
#if 0
            currOutput->Inputs
                = realloc(currOutput->Inputs, sizeof(*devices) * currOutput->nInputs);
            currOutput->Inputs[currOutput->nInputs - 1] = deviceP;
#endif

            i += 3;
        }
    }

    //
    // Part One
    //

    int you = YOU, out = OUT;
#if 1
    findPaths1(bsearch(&you, devices, numDev, sizeof(*devices), &cmpIntDev),
        bsearch(&out, devices, numDev, sizeof(*devices), &cmpIntDev), &result);
    printf("Part one: %llu\n", result);
#endif

    //
    // Part two
    //
    int svr = SVR;
    // harper:ignore
    // result = findPaths2(bsearch(&svr, devices, numDev, sizeof(*devices), &cmpIntDev),
    //     bsearch(&out, devices, numDev, sizeof(*devices), &cmpIntDev), false, false);
    // printf("Part two: %llu\n", result);

    // Topological sort device graph using Kahn's Algorithm (naming convention from Wikipedia)
    device_t* L[numDev] = {};
    device_t* S[numDev] = {};
    int nS = 1;
    int nL = 0;
    S[0] = bsearch(&svr, devices, numDev, sizeof(*devices), &cmpIntDev);

    // We modify / remove the input information from the devices but this isn't a problem because we
    // only need them here
    while (nS) {
        nS--;
        L[nL] = S[nS];
        for (i = 0; i < L[nL]->nOutputs; i++) {
            // Doesn't matter which input pointer we actually cut off
            L[nL]->Outputs[i]->nInputs--;
            if (!L[nL]->Outputs[i]->nInputs) {
                S[nS] = L[nL]->Outputs[i];
                nS++;
            }
        }
        nL++;
    }

    // Explore whole path for every device in _reverse_ topological order
    L[numDev - 1]->outPaths = 1;
    // -1 because out is handled manually and second -1 is obvious
    for (j = numDev - 2; j >= 0; j--) {
        for (i = 0; i < L[j]->nOutputs; i++) {
            L[j]->outPaths += L[j]->Outputs[i]->outPaths;
            L[j]->outPathsDac += L[j]->Outputs[i]->outPathsDac;
            L[j]->outPathsFft += L[j]->Outputs[i]->outPathsFft;
            L[j]->outPathsDacFft += L[j]->Outputs[i]->outPathsDacFft;
        }
        if (L[j]->serial == FFT) {
            L[j]->outPathsFft += L[j]->outPaths;
            L[j]->outPaths = 0;
            L[j]->outPathsDacFft += L[j]->outPathsDac;
            L[j]->outPathsDac = 0;
        } else if (L[j]->serial == DAC) {
            L[j]->outPathsDac += L[j]->outPaths;
            L[j]->outPaths = 0;
            L[j]->outPathsDacFft += L[j]->outPathsFft;
            L[j]->outPathsFft = 0;
        }
    }
    printf("Part two: %llu\n", L[0]->outPathsDacFft);
    for (i = 0; i < numDev; i++) {
        free(devices[i].Outputs);
    }
}
