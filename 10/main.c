// Input file inspection:

// Max 10 control lights (x31) Regex: \[[\.\#]{10}\]
// Max 13 buttons (x5) Regex: (\([\d,]*\)\s){13}
// Max 9 switches per button (x9) Regex: \((\d+,?){9}\)
// Max 10 joltages (identical to control lights) (x31) Regex: \{(\d+,?){10}\}
// Joltage value of 0 possible (x5) Regex: \{(\d+,)+0
// Jolatge maximum of 3 digits (dec) (x49) Regex: \{(\d+,)*\d{3}

#include <ctype.h>
#include <stdio.h>

#ifdef USE_GLPK
#include <glpk.h>
#endif

#define MAX_BUTTONS 13
#define MAX_BUTTON_SIZE 10
#define MAX_INDICATOR_LIGHTS 10
#define MAX_JOLTAGE MAX_INDICATOR_LIGHTS

typedef struct {
    unsigned short indicator_lights;
    unsigned short buttons[MAX_BUTTONS];
    unsigned short nButtons;
    unsigned short buttonSize;
    short joltages[MAX_JOLTAGE];

} machine_t;

int main()
{
    //
    // Parse input
    //

    char input[] = {
#embed "input"
        // #embed "testinput"
        , '\0'
    };
    int i, j, k, indicatorOffset, joltageOffset, numMachines = 0;
    for (i = 0; input[i]; i++) {
        if (input[i] == '\n') numMachines++;
    }
    machine_t machines[numMachines] = {};
    machine_t* machineP = machines;

    for (i = 0; input[i]; i += 2) {
        // Control lights
        for (i++, indicatorOffset = 0; input[i] != ']'; i++, indicatorOffset++) {
            if (input[i] == '#') machineP->indicator_lights |= 1 << indicatorOffset;
        }
        i += 2;
        machineP->buttonSize = 0;
        // Buttons
        for (; input[i] != '{'; i++) {
            // Assumes only single digit size of button (which is true)
            if (isdigit(input[i])) {
                machineP->buttons[machineP->nButtons] |= 1 << (input[i] - '0');
                if (input[i] - '0' >= machineP->buttonSize)
                    machineP->buttonSize = input[i] - '0' + 1;
            } else if (input[i] == ')')
                machineP->nButtons++;
        }

        // Joltages
        for (i++, joltageOffset = 0; input[i] != '}'; i++) {
            if (isdigit(input[i])) {
                machineP->joltages[joltageOffset] *= 10;
                machineP->joltages[joltageOffset] += input[i] - '0';
            } else if (input[i] == ',') {
                joltageOffset++;
            }
        }
        joltageOffset++;
        if (joltageOffset < MAX_JOLTAGE) machineP->joltages[joltageOffset] = -1;
        machineP++;
    }

    //
    // Part one
    //
    // I could reuse the logic from Part two (which I've written after obviously) here to imrpove
    // performance theoretically, but I doubt the difference would be siginificant because of the
    // whole overhead with setting up the linear optimization problems etc. _for such small inputs_.
    // If we had bigger inputs (e.g. number of buttons) changing the approach would be appropriate
    //
    //
    // Observations:
    // - Each button has either to be pressed once or never
    // - order of buttons pressed is irrelevant
    // -> 2^n possible combinations (n is number of buttons)
    //
    // Approach:
    // TBC

    unsigned int result = 0, minSteps, curSteps, curResult;
    for (i = 0; i < numMachines; i++) {
        // I've let ChatGPT rate thw whole code (_after_ completing it) and it recommended UINT_MAX
        // instead of ~0 but idk, I like my solution (Code is unreadable anyways especially if
        // reader isn't used to bitwise logic)
        minSteps = ~0;
        for (j = 1; j < 1 << machines[i].nButtons; j++) {
            curResult = 0;
            curSteps = 0;
            for (k = 0; k < machines[i].nButtons; k++) {
                if (j & (1 << k)) {
                    curResult ^= machines[i].buttons[k];
                    curSteps++;
                }
            }
            if (curResult == machines[i].indicator_lights) {
                if (minSteps > curSteps) minSteps = curSteps;
            }
        }
        result += minSteps;
    }
    printf("Part one: %u\n", result);

    // Part two

    // We treat it as a linear combination, vector has (number of counters)-dimensions
    // each vector represents one button
    // result vector are joltages

    // For overwie on how to use GLPK see poc, example file (both in repo) and refrence manual

#ifdef USE_GLPK
    glp_prob* lp;
    // Indeces of arrays for glpk api start by 1, see refrence manual 2.1.4 (idk why)
    int ia[MAX_BUTTONS * MAX_BUTTON_SIZE + 1], ja[MAX_BUTTONS * MAX_BUTTON_SIZE + 1], currIndex;
    double ar[MAX_BUTTONS * MAX_BUTTON_SIZE + 1], z;
    result = 0;
    for (i = 0; i < numMachines; i++) {
        currIndex = 0;
        lp = glp_create_prob();
        glp_set_obj_dir(lp, GLP_MIN);

        glp_add_rows(lp, machines[i].buttonSize);
        for (j = 0; j < machines[i].buttonSize; j++)
            glp_set_row_bnds(lp, j + 1, GLP_FX, machines[i].joltages[j], machines[i].joltages[j]);

        glp_add_cols(lp, machines[i].nButtons);
        for (j = 1; j <= machines[i].nButtons; j++) {
            // sIx SeVeN
            glp_set_col_bnds(lp, j, GLP_LO, 0, 67);
            glp_set_col_kind(lp, j, GLP_IV);
            glp_set_obj_coef(lp, j, 1);
        }

        for (j = 1; j <= machines[i].buttonSize; j++) {
            for (k = 1; k <= machines[i].nButtons; k++) {
                currIndex++;
                ia[currIndex] = j;
                ja[currIndex] = k;
                ar[currIndex] = machines[i].buttons[k - 1] & (1 << (j - 1)) ? 1 : 0;
            }
        }

        glp_load_matrix(lp, currIndex, ia, ja, ar);

        // Lots of output noise without changing settings
        // Also presolver isn't activated by default for some reason

        glp_iocp solverSettings;
        glp_init_iocp(&solverSettings);
        solverSettings.msg_lev = GLP_MSG_ERR;
        solverSettings.presolve = GLP_ON;
        glp_intopt(lp, &solverSettings);

        // turns out glp_get_obj_val does not return integer result
        z = glp_mip_obj_val(lp);
        glp_delete_prob(lp);
        result += z;
    }
    printf("Part two: %u\n", result);
#else
    printf("Part two requires GLPK, if it installed and make is used, run make using USE_GLPK=1 "
           "(or define it in the Makefile)\n");
#endif
}
