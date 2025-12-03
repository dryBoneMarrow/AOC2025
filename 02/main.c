// #include <math.h>
#include <stdio.h>
#include <string.h>

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
    if (!input) {
        fprintf(stderr, "can't open input file\n");
        return 1;
    }
    unsigned long long int start, end, i, password = 0;
    int digNum;

    //
    // Part One
    //

    // Approach of part two imo cleaner
    while (fscanf(input, "%llu-%llu,", &start, &end) != EOF) {
        for (i = start; i <= end; i++) {
            digNum = 1;
            while ((long long int)(i / pow10int(digNum)))
                digNum++;
            if (digNum % 2) continue;
            if ((long long int)(i / pow10int((int)(digNum / 2)))
                == (long long int)(i % (int)pow10int((int)(digNum / 2)))) {
                password += i;
                // printf("%llu\n", i);
            }
        }
    }
    printf("Part One: %llu\n", password);

    //
    // Part two
    //

    rewind(input);
    password = 0;
    char snum[30];
    int j, k, l, identical;

    // For every range
    while (fscanf(input, "%llu-%llu,", &start, &end) != EOF) {
        // For every number in range
        for (i = start; i <= end; i++) {
            sprintf(snum, "%llu", i);
            digNum = strlen(snum);
            // For every possible subdivision of number
            for (j = 2; j <= digNum; j++) {
                identical = 1;
                if (digNum % j) continue;

                // For every digit per subdivision (e.g. 3rd of each divison)
                for (k = 0; k < (int)digNum / j; k++) {
                    // For evey subdivision
                    for (l = 1; l < j; l++) {
                        if (snum[k + (l - 1) * digNum / j] != snum[k + l * digNum / j]) {
                            identical = 0;
                        }
                    }
                }
                if (identical) {
                    // printf("%llu\n", i);
                    password += i;
                    break;
                }
            }
        }
    }
    printf("Part two: %llu\n", password);
}
