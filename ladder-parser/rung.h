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

#ifndef RUNG_H_
#define RUNG_H_

#include "cstr.h"

typedef struct rung {
      cstr *line;
       int line_qty;
       int line_len;
} rung_t;

void ladder_rung_init(rung_t **rung);
void ladder_rung_destroy(rung_t **rung);
void ladder_rung_add_line(rung_t **rung, char *line);
void ladder_rung_print(rung_t *rung);

#endif /* RUNG_H_ */
