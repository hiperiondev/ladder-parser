/*
 * Copyright 2022 Emiliano Gonzalez (egonzalez . hiperion @ gmail . com))
 * * Project Site: https://github.com/hiperiondev/ladder-parser *
 *
 * This is based on other projects *
 *     please contact their authors for more information.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "rung.h"
#include "cstr.h"
#include "utils.h"
#include "ladder_parser.h"

void print_result(cstr *result_in, cstr *result_out, int result_in_qty, int result_out_qty, cstr *fn, int fn_qty) {
    int n;
    cstr postfix;

    printf("\n--------- FINAL RESULT ---------\n\n");

    printf("[IN] \n");
    for (n = 0; n < result_in_qty; n++) {
        if (cstr_size(result_in[n]) > 0) {
            ladder_remove_redundant_parentheses(&result_in[n]);
            printf("      INFIX: %s\n", result_in[n].str);
            printf("    POSTFIX: ");
            postfix = cstr_with_capacity(1);
            ladder_to_postfix(&postfix, result_in[n].str);
            printf("%s\n\n", postfix.str);
            cstr_drop(&postfix);
        }
    }

    printf("\n[OUT] \n");
    for (n = 0; n < result_out_qty; n++) {
        if (cstr_size(result_out[n]) > 0) {
            ladder_remove_redundant_parentheses(&result_out[n]);
            printf("      INFIX: %s\n", result_out[n].str);
            printf("    POSTFIX: ");
            postfix = cstr_with_capacity(1);
            ladder_to_postfix(&postfix, result_out[n].str);
            printf("%s\n\n", postfix.str);
            cstr_drop(&postfix);
        }
    }
    printf("\n[FUNCTIONS] \n");
    for (n = 0; n < fn_qty; n++) {
        if (cstr_size(fn[n]) > 0) {
            printf("         FN: %s\n", fn[n].str);
        }
    }

    printf("\n--------------------------------\n\n");
}

void free_results(cstr **in, cstr **out, cstr **fn, int in_qty, int out_qty, int fn_qty) {
    int n;

    for (n = 0; n < in_qty; n++)
        cstr_drop(&(*in)[n]);

    for (n = 0; n < out_qty; n++)
            cstr_drop(&(*out)[n]);

    for (n = 0; n < fn_qty; n++)
            cstr_drop(&(*fn)[n]);
}

int main(void) {
    cstr *in, *out, *fn;
    int in_qty, out_qty, fn_qty;
    rung_t *rung;

    printf("-----------------< TEST 1 >----------------\n\n");
    ladder_rung_init(&rung);

    ladder_rung_add_line(&rung, "A--+--+--/B--+--C--+--+--+---Q");
    ladder_rung_add_line(&rung, "   |  |      |     |  |  |               ");
    ladder_rung_add_line(&rung, "D--+  |      +--E--+  |  |     ");
    ladder_rung_add_line(&rung, "   |  |      |     |  |  |         ");
    ladder_rung_add_line(&rung, "F--+  |      +--G--+  |  |     ");
    ladder_rung_add_line(&rung, "      +---H-----I-----+  |     ");
    ladder_rung_add_line(&rung, "J------------------------+                     ");
    ladder_rung_add_line(&rung, "                      ");

    printf("- rung -\n");
    ladder_rung_print(rung);
    ladder_parse(&in, &out, &in_qty, &out_qty, &fn, &fn_qty, &rung);
    print_result(in, out, in_qty, out_qty, fn, fn_qty);

    ladder_rung_destroy(&rung);
    free_results(&in, &out, &fn, in_qty, out_qty, fn_qty);

    /////////////////////////

    printf("-----------------< TEST 2 >----------------\n\n");
    ladder_rung_init(&rung);

    ladder_rung_add_line(&rung, "A--+--+--/B--+--C--+-----+---Q ");
    ladder_rung_add_line(&rung, "   |  |      |     |     |     ");
    ladder_rung_add_line(&rung, "D--+  |      +--E--+     |     ");
    ladder_rung_add_line(&rung, "   |  |      |     |     |     ");
    ladder_rung_add_line(&rung, "F--+  |      +--G--+     |     ");
    ladder_rung_add_line(&rung, "   |  +--------/H--------+     ");
    ladder_rung_add_line(&rung, "I--+---------------------+     ");

    printf("- rung -\n");
    ladder_rung_print(rung);
    ladder_parse(&in, &out, &in_qty, &out_qty, &fn, &fn_qty, &rung);
    print_result(in, out, in_qty, out_qty, fn, fn_qty);

    ladder_rung_destroy(&rung);
    free_results(&in, &out, &fn, in_qty, out_qty, fn_qty);

    /////////////////////////

    printf("-------------------< TEST 3 >-------------------\n\n");
    ladder_rung_init(&rung);

    ladder_rung_add_line(&rung, " A--+--+--/B--+--C--+----+---Q ");
    ladder_rung_add_line(&rung, "    |  |      |     |    |     ");
    ladder_rung_add_line(&rung, " D--+  |      +--E--+    |     ");
    ladder_rung_add_line(&rung, "    |  |      |     |    |     ");
    ladder_rung_add_line(&rung, " F--+  |      +--G--+    |     ");
    ladder_rung_add_line(&rung, "    |  +--------/H-------+     ");
    ladder_rung_add_line(&rung, " I--+----+---J---+---K---+     ");
    ladder_rung_add_line(&rung, "         +---L---+             ");
    ladder_rung_add_line(&rung, " M---------------+             ");

    printf("- rung -\n");
    ladder_rung_print(rung);
    ladder_parse(&in, &out, &in_qty, &out_qty, &fn, &fn_qty, &rung);
    print_result(in, out, in_qty, out_qty, fn, fn_qty);

    ladder_rung_destroy(&rung);
    free_results(&in, &out, &fn, in_qty, out_qty, fn_qty);

    /////////////////////////

    printf("-------------------< TEST 4 >-------------------\n\n");
    ladder_rung_init(&rung);

    ladder_rung_add_line(&rung, " A--+--+-/B---+---C---+-----+--O--+--Q ");
    ladder_rung_add_line(&rung, "    |  |      |       |     |     |    ");
    ladder_rung_add_line(&rung, " D--+  |      +--/E---+     |     +--S ");
    ladder_rung_add_line(&rung, "    |  |      |       |     |          ");
    ladder_rung_add_line(&rung, " F--+  |      +---G---+     |          ");
    ladder_rung_add_line(&rung, "       +---------/H---------+          ");
    ladder_rung_add_line(&rung, " /I----------------+----+---+          ");
    ladder_rung_add_line(&rung, " J-----------------+    |              ");
    ladder_rung_add_line(&rung, " K----L---M---/N--------+              ");

    printf("- rung -\n");
    ladder_rung_print(rung);
    ladder_parse(&in, &out, &in_qty, &out_qty, &fn, &fn_qty, &rung);
    print_result(in, out, in_qty, out_qty, fn, fn_qty);

    ladder_rung_destroy(&rung);
    free_results(&in, &out, &fn, in_qty, out_qty, fn_qty);

    /////////////////////////

    printf("-------------------< TEST 5 >-------------------\n\n");
    ladder_rung_init(&rung);

    ladder_rung_add_line(&rung, " A--+-+-/B-----------------+---C---+---+-----Q ");
    ladder_rung_add_line(&rung, "    | |                    |       |   |       ");
    ladder_rung_add_line(&rung, " D--+ |                    +--/E---+   |       ");
    ladder_rung_add_line(&rung, "    | |                    |       |   |       ");
    ladder_rung_add_line(&rung, " F--+ |                    +---G---+   |       ");
    ladder_rung_add_line(&rung, "      |                    |           |       ");
    ladder_rung_add_line(&rung, "      |   {Func%1_out[1]}--+           |       ");
    ladder_rung_add_line(&rung, "      |  /{Func%1_out[2]}--------------+       ");
    ladder_rung_add_line(&rung, "      +---{Func%1_in[1]}               |       ");
    ladder_rung_add_line(&rung, " /I---P---{Func%1_in[2]}               |       ");
    ladder_rung_add_line(&rung, " J--------{Func%1_in[3]}               |       ");
    ladder_rung_add_line(&rung, "                                       |       ");
    ladder_rung_add_line(&rung, " K----L----M---/N----------------------+       ");

    printf("- rung -\n");
    ladder_rung_print(rung);
    ladder_parse(&in, &out, &in_qty, &out_qty, &fn, &fn_qty, &rung);
    print_result(in, out, in_qty, out_qty, fn, fn_qty);

    ladder_rung_destroy(&rung);
    free_results(&in, &out, &fn, in_qty, out_qty, fn_qty);

    /////////////////////////

    printf("-------------------< TEST 6 >-------------------\n\n");
    ladder_rung_init(&rung);

    ladder_rung_add_line(&rung, "          [10ms]--{ton%4_pt}                               ");
    ladder_rung_add_line(&rung, "    +-------------{ton%4_in}                               ");
    ladder_rung_add_line(&rung, " A--+             {ton%4_q}---------+---C---+------+--Y--Q ");
    ladder_rung_add_line(&rung, "    |                               |       |      |       ");
    ladder_rung_add_line(&rung, " D--+                               +--/E---+      |       ");
    ladder_rung_add_line(&rung, "    |                               |       |      |       ");
    ladder_rung_add_line(&rung, " F--+                               +---G---+      |       ");
    ladder_rung_add_line(&rung, "    |                               |              |       ");
    ladder_rung_add_line(&rung, "    |              /{mux%1_out}-----+              |       ");
    ladder_rung_add_line(&rung, "    +---------------{mux%1_in0}                    |       ");
    ladder_rung_add_line(&rung, " /I---------P-------{mux%1_in1}                    |       ");
    ladder_rung_add_line(&rung, " $a1----------------{mux%1_in2}                    |       ");
    ladder_rung_add_line(&rung, " T------------------{mux%1_in3}                    |       ");
    ladder_rung_add_line(&rung, " _Q-----------------{mux%1_S1}                     |       ");
    ladder_rung_add_line(&rung, "                                                   |       ");
    ladder_rung_add_line(&rung, " K-----L---M---/N--------------------+-----/X------+       ");
    ladder_rung_add_line(&rung, "                                     |             |       ");
    ladder_rung_add_line(&rung, " $b-----{gt%2_in0}                   |             |       ");
    ladder_rung_add_line(&rung, " $c--+--{gt%2_in1}                   |             |       ");
    ladder_rung_add_line(&rung, "     |  {gt%2_out}---+               |             |       ");
    ladder_rung_add_line(&rung, "     |               |               |             |       ");
    ladder_rung_add_line(&rung, "     |               +-{eq%3_en}     |             |       ");
    ladder_rung_add_line(&rung, " V---+-----------------{eq%3_in1}    |             |       ");
    ladder_rung_add_line(&rung, "                                     |             |       ");
    ladder_rung_add_line(&rung, " W---+-----------------{eq%3_in2}    |             |       ");
    ladder_rung_add_line(&rung, "     |                 {eq%3_eno}----+----$b       |       ");
    ladder_rung_add_line(&rung, "     |                 {eq%3_out}------------------+       ");
    ladder_rung_add_line(&rung, " R---+--{mux%1_S0}                                         ");
    ladder_rung_add_line(&rung, "                                                           ");

    printf("- rung -\n");
    ladder_rung_print(rung);
    ladder_parse(&in, &out, &in_qty, &out_qty, &fn, &fn_qty, &rung);
    print_result(in, out, in_qty, out_qty, fn, fn_qty);

    ladder_rung_destroy(&rung);
    free_results(&in, &out, &fn, in_qty, out_qty, fn_qty);

    return EXIT_SUCCESS;
}
