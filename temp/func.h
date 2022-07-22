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

#ifndef FUNC_H_
#define FUNC_H_

#include "cstr.h"

typedef enum {
    NOVARARGS = 0,
    VARARGS = 1,
} varargs_t;

typedef struct func {
    varargs_t varargs;
         char name[254];
          int args_qty;
         char args[254][254];
} func_t;

void ladder_func_init(func_t ***func);
void ladder_func_destroy(func_t ***func);
void ladder_func_add(func_t ***func, char *name, varargs_t vararg, char *args);

#endif /* FUNC_H_ */
