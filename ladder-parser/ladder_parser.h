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

#ifndef LADDER_PARSER_H_
#define LADDER_PARSER_H_

#include "rung.h"

//#define LADDER_DEBUG

enum ladder_type {
    LD_NODE,
    LD_FUNCTION,
    LD_OUTPUT
};
typedef enum ladder_type ladder_type_t;

typedef struct ladder_result {
    ladder_type_t type; // node, function, output
    char *name;
    char *value;
} ladder_result_t;

ladder_result_t* ladder(rung_t **rung, int *result_qty);

#endif /* LADDER_PARSER_H_ */
