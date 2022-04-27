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

#include <stddef.h>

#include "rung.h"

void ladder_rung_init(rung_t **rung) {
    *rung = malloc(sizeof(rung_t));
    (*rung)->line = NULL;
    (*rung)->line_qty = 0;
    (*rung)->line_len = 0;
}

void ladder_rung_destroy(rung_t **rung) { // TODO: BAD!!!! check
    //for (int n = 0; n < (*rung)->line_qty; n++) {
    //    cstr_drop(&((*rung)->line[n]));
    //}
    free((*rung)->line);
    free(*rung);
}

void ladder_rung_add_line(rung_t **rung, char *line) {
    if ((*rung)->line) {
        (*rung)->line = realloc((*rung)->line, ((*rung)->line_qty + 1) * sizeof(cstr));
    } else {
        (*rung)->line = malloc(sizeof(cstr));
    }

    (*rung)->line[(*rung)->line_qty] = cstr_with_capacity(1);
    cstr_assign(&((*rung)->line[(*rung)->line_qty]), line);

    if (!cstr_ends_with((*rung)->line[(*rung)->line_qty], " "))
        cstr_append(&((*rung)->line[(*rung)->line_qty]), " ");

    if (cstr_length((*rung)->line[(*rung)->line_qty]) > (*rung)->line_len) {
        (*rung)->line_len = cstr_length((*rung)->line[(*rung)->line_qty]);

        // pad lines
        for (int n = 0; n < (*rung)->line_qty; n++) {
            int len = cstr_length((*rung)->line[n]);
            if (len < (*rung)->line_len) {
                for (int m = 0; m < (*rung)->line_len - len; m++) {
                    cstr_append(&(*rung)->line[n], " ");
                }
            }
        }
    }
    ++((*rung)->line_qty);
}

void ladder_rung_print(rung_t *rung) {
    for (int n = 0; n < rung->line_qty; n++) {
        printf("%s\n", rung->line[n].str);
    }
}
