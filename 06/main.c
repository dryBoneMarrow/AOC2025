#include <stdio.h>
#include <stdlib.h>

// No stdio.h except for printing because why not :)

long unsigned long multiply(unsigned short* numbers, int n)
{
    unsigned long long result = 1;
    for (int i = 0; i < n; i++) {
        result *= numbers[i];
    }
    return result;
}

long unsigned long add(unsigned short* numbers, int n)
{
    unsigned long long result = 0;
    for (int i = 0; i < n; i++) {
        result += numbers[i];
    }
    return result;
}

typedef struct {
    unsigned short* numbers;
    int n;
    long long unsigned (*calculate)(unsigned short*, int);
} mathProblem_t;

int main()
{
    const char input[] = {
#embed "input"
        , '\0'
    };

    //
    // Part one
    // (some parts reused in part two)

    // Count number of lines
    int i, line_count = 0;
    for (i = 0; input[i]; i++)
        if (input[i] == '\n') line_count++;

    // Count number of math problems
    int problems_count = 0;
    bool isNewNumber = true;
    for (i = 0; input[i] != '\n'; i++) {
        if (input[i] == ' ')
            isNewNumber = true;
        else if (isNewNumber) {
            problems_count++;
            isNewNumber = false;
        }
    }

    // Create math problems and fill them with already known information
    mathProblem_t problems[problems_count] = {};
    for (i = 0; i < problems_count; i++) {
        problems[i].n = line_count - 1;
        problems[i].numbers = calloc(line_count - 1, sizeof(unsigned short));
        if (!problems[i].numbers) {
            fprintf(stderr, "Couldn't allocate memory :(\n");
            return 42;
        }
    }

    // Fill math problems with numbers
    mathProblem_t* problems_p = problems - 1;
    isNewNumber = true;
    int currLine = 0;

    for (i = 0; input[i] != '*' && input[i] != '+'; i++) {
        if (input[i] == ' ')
            isNewNumber = true;
        else if (input[i] == '\n') {
            isNewNumber = true;
            problems_p = problems - 1;
            currLine++;
        } else if (isNewNumber) {
            problems_p++;
            problems_p->numbers[currLine] += (unsigned short)(input[i] - '0');
            isNewNumber = false;
        } else {
            problems_p->numbers[currLine] *= 10;
            problems_p->numbers[currLine] += (unsigned short)(input[i] - '0');
        }
    }

    // Fill math problem with operators
    for (problems_p = problems; input[i]; i++) {
        if (input[i] == '+') {
            problems_p->calculate = &add;
            problems_p++;
        } else if (input[i] == '*') {
            problems_p->calculate = &multiply;
            problems_p++;
        }
    }

    // Solve and add problems
    long long unsigned grandTotal = 0;
    for (i = 0; i < problems_count; i++) {
        // Here C++ Objects would be useful
        grandTotal += problems[i].calculate(problems[i].numbers, problems[i].n);
    }

    printf("Part one: %llu\n", grandTotal);

    //
    // Part two
    //

    // Get length of line
    int lineLength;
    for (lineLength = 0; input[lineLength] != '\n'; lineLength++)
        ;
    lineLength++;

    // Reset problems
    // Operators don't _have_ to be reset but as it is very easy to reassign them in the second part
    // I do this so that both parts work independently
    int j;
    for (i = 0; i < problems_count; i++) {
        for (j = 0; j < line_count - 1; j++) {
            problems[i].numbers[j] = 0;
            problems[i].calculate = nullptr;
        }
    }

    // Parse input from left to right column-wise
    int nNum = -1;
    char currChar;
    problems_p = problems + problems_count - 1;
    for (i = lineLength - 2; i >= 0; i--) {
        nNum++;
        for (j = 0; j < line_count; j++) {
            currChar = input[i + lineLength * j];
            if (currChar == '+' || currChar == '*') {
                // Each problem has already been assigned the correct operator in part one
                // (not assigning it again because eFfIcIeNcY)
                problems_p->calculate = currChar == '+' ? add : multiply;
                problems_p->n = nNum + 1;
                problems_p--;
                nNum = -1;
                i--;
            } else if (currChar == ' ')
                continue;
            else {
                problems_p->numbers[nNum] *= 10;
                problems_p->numbers[nNum] += currChar - '0';
            }
        }
    }

    // Solve and add problems
    grandTotal = 0;
    for (i = 0; i < problems_count; i++) {
        // Here C++ Objects would be useful
        grandTotal += problems[i].calculate(problems[i].numbers, problems[i].n);
    }

    printf("Part two: %llu\n", grandTotal);

    // Free heap memory
    for (i = 0; i < problems_count; i++) {
        free(problems[i].numbers);
    }
}
