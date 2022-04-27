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
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>

#include "cstr.h"
#define i_type iStack
#define i_val int
#include "cstack.h"

#define STACK_SIZE 1024

typedef uint8_t u8;
typedef struct Stack Stack;

struct Stack {
    u8 *array;
    size_t size;
    size_t stackptr;
};

static char* infix_to_postfix(const char *inf, size_t len);

void ladder_remove_redundant_parentheses(cstr *expression) {
    char c[2];
    cstr result;

    c[1] = '\0';
    result = cstr_with_capacity(1);

    // Assume given expression
    int n = cstr_length(*expression);
    if (n == 0) {
        printf("err: length 0\n");
        return;
    }

    // Valid Parentheses checker
    bool check[n];
    for (int i = 0; i < n; ++i) {
        check[i] = true;
    }

    // Create an empty record
    iStack record = iStack_init();

    // Execute loop through by size of n
    for (int i = 0; i < n; ++i) {
        if (expression->str[i] == ')') {
            if (iStack_empty(record)) {
                // Expression are not valid
                printf("err: Expression are not valid\n");
                return;
            } else if (expression->str[*iStack_top(&record)] == '(') {
                check[i] = false;
                check[*iStack_top(&record)] = false;
                // Remove top element of stacks
                iStack_pop(&record);
            } else {
                while (iStack_empty(record) == false && expression->str[*iStack_top(&record)] != '(') {
                    // Remove element until not get open parentheses
                    iStack_pop(&record);
                }
                if (iStack_empty(record)) {
                    // Expression are not valid
                    printf("err: Expression are not valid\n");
                    return;
                }
                iStack_pop(&record);
            }
        } else {
            // Insert position of expression character
            iStack_push(&record, i);
        }
    }

    // This is collects resultant expression
    for (int j = 0; j < n; ++j) {
        if (check[j]) {
            c[0] = expression->str[j];
            cstr_append(&result, c);
        }
    }

    cstr_assign(expression, result.str);
}

void ladder_to_postfix(cstr *postfix, char *eqn) {
    char *pfix = NULL;
    int pos;

    size_t len = strlen(eqn);
    pfix = infix_to_postfix(eqn, len);
    cstr_assign(postfix, pfix);

    cstr_replace_all(postfix, "&", " and ");
    cstr_replace_all(postfix, "|", " or ");
    cstr_replace_all(postfix, "!", " not ");
    while ((pos = cstr_find(*postfix, "  ")) > 0)
        cstr_erase(postfix, pos);
}

static int push(Stack *sp, u8 i) {
    if (sp->stackptr >= sp->size - 1)
        return -1;
    sp->array[sp->stackptr++] = i;
    return 0;
}

static int pop(Stack *sp, u8 *p) {
    if (sp->stackptr == 0)
        return -1;
    *p = sp->array[--sp->stackptr];
    return 0;
}

static inline int is_operator(int c) {
    return (c == '&' || c == '|' || c == '!');
}

static int create_stack(size_t size, Stack **sp) {
    Stack *s = malloc(sizeof(*s));
    if (!s)
        return -1;
    if (!(s->array = malloc(size))) {
        free(s);
        return -1;
    }

    s->size = size;
    s->stackptr = 0;
    *sp = s;
    return 0;
}

static void destroy_stack(Stack **s) {
    free((*s)->array);
    free(*s);
    *s = NULL;
}

static char* infix_to_postfix(const char *inf, size_t len) {
    Stack *postfix, *operators;
    if (create_stack(len + 1, &postfix) || create_stack(len, &operators))
        return NULL;

    static const int precedences[256] = {
            ['&'] = 1,
            ['|'] = 1,
            ['!'] = 2,
            ['('] = INT_MIN,
            [')'] = INT_MIN
    };

    for (; *inf; ++inf) {
        if (is_operator(*inf)) {
            u8 i;
            while (!pop(operators, &i)) {
                if (precedences[i] < precedences[(int) *inf]) {
                    push(operators, i);
                    break;
                }
                push(postfix, i);
            }
            push(operators, *inf);
        } else if (*inf == '(') {
            push(operators, *inf);
        } else if (*inf == ')') {
            u8 i;
            while (!pop(operators, &i)) {
                if (i == '(')
                    break;
                push(postfix, i);
            }
        } else {
            push(postfix, *inf);
        }
    }

    char *s = (char*) postfix->array;
    u8 i;
    while (!pop(operators, &i)) {
        push(postfix, i);
    }
    push(postfix, 0);
    free(postfix);
    destroy_stack(&operators);
    return s;
}
