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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "func.h"
#include "ladder_parser.h"

int funcs_qty = 0;

void ladder_func_init(func_t ***func) {
    *func = malloc(sizeof(func_t*));
}

void ladder_func_destroy(func_t ***func) {

}

void ladder_func_add(func_t ***func, char *name, varargs_t vararg, char *args) {
    int q = 0;

    memcpy((*func)[funcs_qty]->name, name, strlen(name) + 1);

    (*func)[funcs_qty]->varargs = vararg;

    char delim[] = " ";
    char *ptr = strtok(args, delim);

    while (ptr != NULL) {
        memcpy((*func)[funcs_qty]->args[q], ptr, strlen(ptr) + 1);
        ptr = strtok(NULL, delim);
        q++;
    }

    (*func)[funcs_qty]->args_qty = q;
    funcs_qty++;

    *func = realloc(*func, sizeof(func_t*) * (funcs_qty + 1));
        (*func)[funcs_qty] = malloc(sizeof(func_t));

}



