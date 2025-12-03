#include <stdio.h>
#include <time.h>

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

    // Benchmark between inefficient (first) and faster but more complicated (second) approach
    clock_t start = clock();

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

    clock_t end1 = clock();

    printf("Part Two(I):  %d (Ticks:%llu)\n", counter, (long long unsigned)(end1 - start));

    //
    // Part two nicer
    //
    position = 50, counter = 0;
    rewind(input);
    while (fgets(curLine, 6, input)) {
        sscanf(curLine, "%c%d", &instruction.direction, &instruction.turns);
        // printf(" %d\n%c%d:", position, instruction.direction, instruction.turns);
        switch (instruction.direction) {
        case 'L':
            while (instruction.turns >= 100) {
                instruction.turns -= 100;
                counter++;
                // printf("&&");
            }
            if (instruction.turns == 0) continue;
            if (position == 0) counter--;
            if (position - instruction.turns > 0) {
                position -= instruction.turns;
                continue;
            }
            if (position - instruction.turns == 0) {
                position -= instruction.turns;
                counter++;
                // printf("&&");
                continue;
            }
            if (position - instruction.turns < 0) {
                counter++;
                // printf("&&");
                position += 100 - instruction.turns;
                continue;
            }
            puts("What the helly");

            // The following 3 lines are taken from ChatGPT after instructiong it to rate my code,
            // unsurprisingly they are wrong per se (had to modify them, semicolons are hard) and on
            // top of that calculate the wrong result (is quite near though). But ofc, it's solution
            // is better

            // counter += (instruction.turns - position + 99) / 100;
            // position = (position - instruction.turns % 100 + 100) % 100;
            // break;

        case 'R':
            while (instruction.turns > 100) {
                instruction.turns -= 100;
                counter++;
                // printf("&&");
            }
            if (instruction.turns == 0) continue;
            if (position + instruction.turns < 100) {
                position += instruction.turns;
                continue;
            }
            if (position + instruction.turns >= 100) {
                counter++;
                // printf("&&");
                position -= 100 - instruction.turns;
                continue;
            }
            puts("What the helly");

            // The following 3 lines are taken from ChatGPT after instructiong it to rate my code,
            // unsurprisingly they are wrong per se (had to modify them, semicolons are hard) and on
            // top of that calculate the wrong result (is quite near though). But ofc, it's solution
            // is better

            // counter += (position +instruction.turns) / 100;
            // position = (position + instruction.turns) % 100;
            // break;
        default: puts("What the helly");
        }
    }
    // printf("%d\n", position);

    clock_t end2 = clock();

    printf("Part Two(II): %d (Ticks:%llu)\n", counter, (long long unsigned)(end2 - end1));
    printf("=> Second approach to Part two is %.2Lf%% faster :)\n",
        (long double)(end1 - start) / (long double)(end2 - end1) * 100 - 100);

    fclose(input);
    return 0;
}
