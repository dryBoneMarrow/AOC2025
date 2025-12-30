#include <glpk.h>
#include <stdio.h>

int main()
{
    // poc (works) for the first machine of testinput
    // see anotated example_glpk.c for more clarity

    // Names are commented out because useless (if no need to extract them) but more clear for POC
    // (uses same naming shemes for rows as example and abc... instead of x_n for columns)
    glp_prob* lp;
    int ia[24 + 1], ja[24 + 1]; // Elements in constraint matrix
    double ar[24 + 1], z;

    lp = glp_create_prob();
    // glp_set_prob_name(lp, "testinput_1");
    glp_set_obj_dir(lp, GLP_MIN);

    glp_add_rows(lp, 4);

    // glp_set_row_name(lp, 1, "p");
    glp_set_row_bnds(lp, 1, GLP_FX, 3, 3);

    // glp_set_row_name(lp, 2, "q");
    glp_set_row_bnds(lp, 2, GLP_FX, 5, 5);

    // glp_set_row_name(lp, 3, "r");
    glp_set_row_bnds(lp, 3, GLP_FX, 4, 4);

    // glp_set_row_name(lp, 4, "s");
    glp_set_row_bnds(lp, 4, GLP_FX, 7, 7);

    glp_add_cols(lp, 6);

    // glp_set_col_name(lp, 1, "a");
    glp_set_col_bnds(lp, 1, GLP_LO, 0, 0);
    glp_set_col_kind(lp, 1, GLP_IV);
    // Weirdly enough coefficients aren't 1 by default (!)
    glp_set_obj_coef(lp, 1, 1);

    // glp_set_col_name(lp, 2, "b");
    glp_set_col_bnds(lp, 2, GLP_LO, 0, 0);
    glp_set_col_kind(lp, 2, GLP_IV);
    glp_set_obj_coef(lp, 2, 1);

    // glp_set_col_name(lp, 3, "c");
    glp_set_col_bnds(lp, 3, GLP_LO, 0, 0);
    glp_set_col_kind(lp, 3, GLP_IV);
    glp_set_obj_coef(lp, 3, 1);

    // glp_set_col_name(lp, 4, "d");
    glp_set_col_bnds(lp, 4, GLP_LO, 0, 0);
    glp_set_col_kind(lp, 4, GLP_IV);
    glp_set_obj_coef(lp, 4, 1);

    // glp_set_col_name(lp, 5, "e");
    glp_set_col_bnds(lp, 5, GLP_LO, 0, 0);
    glp_set_col_kind(lp, 5, GLP_IV);
    glp_set_obj_coef(lp, 5, 1);

    // glp_set_col_name(lp, 6, "f");
    glp_set_col_bnds(lp, 6, GLP_LO, 0, 0);
    glp_set_col_kind(lp, 6, GLP_IV);
    glp_set_obj_coef(lp, 6, 1);

    // ia=y, ja=x
    ia[1] = 1, ja[1] = 1, ar[1] = 0;
    ia[2] = 1, ja[2] = 2, ar[2] = 0;
    ia[3] = 1, ja[3] = 3, ar[3] = 0;
    ia[4] = 1, ja[4] = 4, ar[4] = 0;
    ia[5] = 1, ja[5] = 5, ar[5] = 1;
    ia[6] = 1, ja[6] = 6, ar[6] = 1;

    ia[7] = 2, ja[7] = 1, ar[7] = 0;
    ia[8] = 2, ja[8] = 2, ar[8] = 1;
    ia[9] = 2, ja[9] = 3, ar[9] = 0;
    ia[10] = 2, ja[10] = 4, ar[10] = 0;
    ia[11] = 2, ja[11] = 5, ar[11] = 0;
    ia[12] = 2, ja[12] = 6, ar[12] = 1;

    ia[13] = 3, ja[13] = 1, ar[13] = 0;
    ia[14] = 3, ja[14] = 2, ar[14] = 0;
    ia[15] = 3, ja[15] = 3, ar[15] = 1;
    ia[16] = 3, ja[16] = 4, ar[16] = 1;
    ia[17] = 3, ja[17] = 5, ar[17] = 1;
    ia[18] = 3, ja[18] = 6, ar[18] = 0;

    ia[19] = 4, ja[19] = 1, ar[19] = 1;
    ia[20] = 4, ja[20] = 2, ar[20] = 1;
    ia[21] = 4, ja[21] = 3, ar[21] = 0;
    ia[22] = 4, ja[22] = 4, ar[22] = 1;
    ia[23] = 4, ja[23] = 5, ar[23] = 0;
    ia[24] = 4, ja[24] = 6, ar[24] = 0;

    glp_load_matrix(lp, 24, ia, ja, ar);
    glp_smcp solverSettings;
    glp_init_smcp(&solverSettings);
    solverSettings.msg_lev = GLP_MSG_ERR;
    glp_simplex(lp, &solverSettings);

    z = glp_get_obj_val(lp);
    glp_delete_prob(lp);
    printf("\n%f\n", z);
}
