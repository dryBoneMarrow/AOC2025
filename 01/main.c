#include <stdio.h>

typedef struct {
    char direction;
    int turns;
} instruction_t;

int main()
{
    int position = 50, counter = 0, i;
    FILE* input = fopen("input", "r");
    char curLine[6]; // Checked for longest line in input
    instruction_t instruction;

    //
    // Part one
    //
    if (!input) {
        fprintf(stderr, "can't open input file\n");
        return 1;
    }

    while (fgets(curLine, 6, input)) {
        sscanf(curLine, "%c%d", &instruction.direction, &instruction.turns);
        switch (instruction.direction) {
        case 'L':
            position = (position - instruction.turns) % 100;
            if (position < 0) position += 100;
            break;
        case 'R': position = (position + instruction.turns) % 100; break;
        }
        if (!position) counter++;
    }
    printf("Part One: %d\n", counter);

    //
    // Part two
    //

    // Inefficient but acceptable for such small input, may improve later (seems to have some edge
    // cases when using % like above)
    rewind(input);
    position = 50, counter = 0;

    while (fgets(curLine, 6, input)) {
        sscanf(curLine, "%c%d", &instruction.direction, &instruction.turns);
        switch (instruction.direction) {
        case 'L':
            for (i = 0; i < instruction.turns; i++) {
                position--;
                if (!position) counter++;
                if (position < 0) position += 100;
            }
            break;
        case 'R':
            for (i = 0; i < instruction.turns; i++) {
                position++;
                if (position == 100) {
                    counter++;
                    position -= 100;
                }
            }
            break;
        }
    }
    printf("Part Two: %d\n", counter);

    fclose(input);
    return 0;
}
