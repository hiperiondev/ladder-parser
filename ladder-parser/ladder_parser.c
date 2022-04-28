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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "cstr.h"
#include "list.h"
#include "ladder_parser.h"
#include "rung.h"

enum CELL_TYPE {
    CELL_NONE,
    CELL_AND,
    CELL_OR,
    CELL_STR,
    CELL_NODE,
    CELL_SPACE,
};

typedef struct cell {
     int type;
    bool neg;
    union {
        char *str;
         int index;
    };
} cell_t;

void ladder_identify_char(rung_t **rung, cell_t ***lns) {
    int line, row, char_pos = 0;
    char c = 0, str[254];
    bool neg = false;
    memset(str, 0, sizeof(str));

    for (line = 0; line < (*rung)->line_qty; line++) {
        for (row = 0; row < (*rung)->line_len; row++) {
            c = (*rung)->line[line].str[row];
            switch (c) {

                case '\n':
                    continue;
                    break;

                case '/':
                    neg = true;
                    continue;
                    break;

                case ' ':
                    (*lns)[line][row].type = CELL_SPACE;
                    if (char_pos != 0) {
                        (*lns)[line][row - 1].type = CELL_STR;
                        (*lns)[line][row - 1].neg = neg;
                        (*lns)[line][row - 1].str = (char*) malloc((strlen(str) + 1) * sizeof(char));
                        sprintf((*lns)[line][row - 1].str, "%s", str);
                        char_pos = 0;
                        neg = false;
                        memset(str, 0, sizeof(str));
                    }
                    continue;
                    break;

                case '|':
                    (*lns)[line][row].type = CELL_OR;
                    continue;
                    break;

                case '-':
                    (*lns)[line][row].type = CELL_AND;
                    if (char_pos != 0) {
                        (*lns)[line][row - 1].type = CELL_STR;
                        (*lns)[line][row - 1].neg = neg;
                        (*lns)[line][row - 1].str = (char*) malloc((strlen(str) + 1) * sizeof(char));
                        sprintf((*lns)[line][row - 1].str, "%s", str);
                        char_pos = 0;
                        neg = false;
                        memset(str, 0, sizeof(str));
                    }
                    continue;
                    break;

                case '+':
                    (*lns)[line][row].type = CELL_NODE;
                    continue;

            }
            str[char_pos++] = c;
        }
    }
}

int ladder_unify_nodes(rung_t **rung, cell_t ***lns) {
    int line, column, type, index, pos = 0, nodes_qty = 1;

    // unify nodes
    for (line = 0; line < (*rung)->line_qty; line++) {
        for (column = 0; column < (*rung)->line_len; column++) {
            type = (*lns)[line][column].type;
            index = (*lns)[line][column].index;

            if (type == CELL_NODE) {
                if (index != 0)
                    continue;

                for (pos = line; pos < (*rung)->line_qty; pos++) {

                    if ((*lns)[pos][column].type == CELL_NODE) {
                        (*lns)[pos][column].index = nodes_qty;
                        continue;
                    }

                    if ((*lns)[pos][column].type == CELL_SPACE || (*lns)[pos][column].type != CELL_OR) {
                        if ((*lns)[pos - 1][column].type != CELL_NODE) {
                            printf("error!\n");
                            exit(1);
                        }

                        ++nodes_qty;
                        break;
                    }
                }
                if (pos == (*rung)->line_qty)
                    ++nodes_qty;
            }
        }
    }

#ifdef LADDER_DEBUG
    printf("\n- ladder_unify_nodes -\n");
    for (line = 0; line < (*rung)->line_qty; line++) {
        for (column = 0; column < (*rung)->line_len; column++) {
            type = (*lns)[line][column].type;
            switch (type) {
                case CELL_NONE:
                    break;

                case CELL_SPACE:
                    printf(" ");
                    break;

                case CELL_AND:
                    printf("-");
                    break;

                case CELL_OR:
                    printf("|");
                    break;

                case CELL_STR:
                    printf("%s%s", (*lns)[line][column].neg ? "/" : "", (*lns)[line][column].str);
                    break;

                case CELL_NODE:
                    printf("%d", (*lns)[line][column].index);
                    break;
            }
        }
        printf("\n");
    }
#endif

    return nodes_qty;
}

int ladder_separate_blocks(cell_t ***blocks_tmp, rung_t **rung, int nodes_qty, cell_t ***lns) { // +--[...]--+
    int line, column, pos, block;
    bool and;

    and = false;
    block = 0;

    (*blocks_tmp) = malloc(1024 * sizeof(cell_t*));
    for (pos = 0; pos < 1024; pos++)
        (*blocks_tmp)[pos] = calloc(1024, sizeof(cell_t));

    pos = 0;

    for (line = 0; line < (*rung)->line_qty; line++) {
        for (column = 0; column < (*rung)->line_len; column++) {

            if ((*lns)[line][column].type == CELL_STR) {
                (*blocks_tmp)[block][pos].type = (*lns)[line][column].type;
                (*blocks_tmp)[block][pos].neg = (*lns)[line][column].neg;
                (*blocks_tmp)[block][pos].str = (char*) malloc((strlen((*lns)[line][column].str) + 1) * sizeof(char));
                strcpy((*blocks_tmp)[block][pos++].str, (*lns)[line][column].str);

                and = false;
                continue;
            }

            if ((*lns)[line][column].type == CELL_AND && !and) {
                (*blocks_tmp)[block][pos++].type = (*lns)[line][column].type;
                and = true;
                continue;
            }

            if ((*lns)[line][column].type == CELL_NODE) {
                (*blocks_tmp)[block][pos].type = CELL_NODE;
                (*blocks_tmp)[block][pos].index = (*lns)[line][column].index;

                ++block;

                pos = 0;
                (*blocks_tmp)[block][pos].type = CELL_NODE;
                (*blocks_tmp)[block][pos++].index = (*lns)[line][column].index;

                and = false;
            }

        }

        if (pos == 1) {
            (*blocks_tmp)[block][0].type = CELL_NONE;
            --block;
        }

        ++block;

        pos = 0;
    }

#ifdef LADDER_DEBUG
    printf("\n-ladder_separate_blocks-\n");
    for (line = 0; line < block; line++) {
        for (column = 0; column < (*rung)->line_len; column++) {
            switch ((*blocks_tmp)[line][column].type) {
                case CELL_NODE:
                    printf("Node(%d)", (*blocks_tmp)[line][column].index);
                    break;
                case CELL_AND:
                    printf("-");
                    break;
                case CELL_STR:
                    printf("[%s]", (*blocks_tmp)[line][column].str);
                    break;
            }
        }
        printf("\n");
    }
#endif

    return block;
}

int ladder_allocate_list_blocks(cell_t ***blocks_tmp, rung_t **rung, list_t ***blocks_list, int blocks_qty) {
    int line, column, block, line_len;
    bool has_block = false;

    block = 0;
    line_len = (*rung)->line_len;

    for (line = 0; line < blocks_qty; line++) {
        (*blocks_list) = realloc((*blocks_list), (block + 1) * sizeof(list_t*));
        (*blocks_list)[block] = list_new();

        for (column = 0; column < line_len; column++) {

            if ((*blocks_tmp)[line][column].type == CELL_STR) {
                has_block = true;
                list_rpush((*blocks_list)[block], list_node_new((void*) (&(*blocks_tmp)[line][column])));
                continue;
            }

            if ((*blocks_tmp)[line][column].type == CELL_NODE) {
                if ((column == 0 && (*blocks_tmp)[line][column + 1].type != CELL_AND) || (column != 0 && (*blocks_tmp)[line][column - 1].type == CELL_NODE)
                        || (column < line_len && (*blocks_tmp)[line][column + 1].type == CELL_NODE))
                    continue;

                has_block = true;
                list_rpush((*blocks_list)[block], list_node_new((void*) (&(*blocks_tmp)[line][column])));
                continue;
            }

            if ((*blocks_tmp)[line][column].type == CELL_AND) {
                has_block = true;
                list_rpush((*blocks_list)[block], list_node_new((void*) (&(*blocks_tmp)[line][column])));
            }
        }
        if (has_block) {
            has_block = false;
            ++block;
        }
    }

#ifdef LADDER_DEBUG
    printf("\n- ladder_allocate_list_blocks -\n");
    list_node_t *node;
    cell_t *item;
    for (int n = 0; n < block; n++) {
        list_iterator_t *it = list_iterator_new((*blocks_list)[n], LIST_HEAD);
        printf("%d) ", n);
        while ((node = list_iterator_next(it))) {
            item = node->val;
            if (item->type == CELL_NODE)
                printf("Node(%d)", item->index);

            if (item->type == CELL_STR)
                printf("[%s%s]", item->neg ? "not " : "", item->str);

            if (item->type == CELL_AND)
                printf("-");
        }
        printf("\n");
    }
    printf("\n");
#endif

    return block;
}

void ladder_join_nodes(list_t ***blocks_list, int nodes_qty, int blocks_qty, cstr **str_in, cstr **str_out, int *str_in_qty, int *str_out_qty) { // join nodes (equal right nodes are OR'ed)
    int block, node;
    bool and, or, has_node;
    list_node_t *node_list;
    list_iterator_t *it;
    cell_t *item;
    cstr str_tmp;

    has_node = false;
    *str_in_qty = 0;
    and = false;

    (*str_in)[0] = cstr_with_capacity(1);
    (*str_out)[0] = cstr_with_capacity(1);
    str_tmp = cstr_with_capacity(1);

    // inputs
    for (node = 0; node <= nodes_qty; node++) {
        or = false;

        for (block = 0; block < blocks_qty; block++) {
            item = list_at((*blocks_list)[block], -1)->val;

            if (item->type == CELL_NODE && item->index == node) {

                if (!has_node) {
                    cstr_printf(&str_tmp, "_N[%d] =", node);
                    cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                }

                if (or) {
                    cstr_printf(&str_tmp,"|");
                    cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                }

                cstr_printf(&str_tmp, " ( ");
                cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                has_node = true;
                or = true;

                it = list_iterator_new((*blocks_list)[block], LIST_HEAD);
                while ((node_list = list_iterator_next(it))) {

                    if (!node_list->next)
                        break;

                    if (and) {
                        cstr_printf(&str_tmp, " & ");
                        cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                        and = false;
                    }

                    item = node_list->val;

                    if (item->type == CELL_NODE) {
                        cstr_printf(&str_tmp, "_N[%d]", item->index);
                        cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                    }

                    if (item->type == CELL_STR) {
                        cstr_printf(&str_tmp, "%s%s", item->neg ? "!" : "", item->str);
                        cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                    }

                    if (item->type == CELL_AND)
                        and = true;
                }

                cstr_printf(&str_tmp, " ) ");
                cstr_append(&((*str_in)[*str_in_qty]), str_tmp.str);
                and = false;
            }
        }

        if (has_node) {
            ++(*str_in_qty);
            *str_in = realloc(*str_in, (*str_in_qty + 1) * sizeof(cstr*));
            (*str_in)[*str_in_qty] = cstr_with_capacity(1);
            has_node = false;
        }
    }

#ifdef LADDER_DEBUG
    printf("- ladder_join_nodes -\n");
    printf("IN (%d):\n", *str_in_qty);
    for (block = 0; block < *str_in_qty; block++)
        if (cstr_length((*str_in)[block]) > 0)
            printf("%s\n", (*str_in)[block].str);
    printf("end IN\n");
#endif

    // outputs
    or = false;
    *str_out_qty = 0;
    for (block = 0; block < blocks_qty; block++) {
        item = list_at((*blocks_list)[block], -1)->val;

        if (item->type == CELL_STR) {
            cstr_printf(&str_tmp, "%s =", item->str);
            cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);

            if (or) {
                cstr_printf(&str_tmp, "|");
                cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);
            }

            cstr_printf(&str_tmp, " ( ");
            cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);
            or = true;

            it = list_iterator_new((*blocks_list)[block], LIST_HEAD);
            while ((node_list = list_iterator_next(it))) {
                if (!node_list->next)
                    break;

                if (and) {
                    cstr_printf(&str_tmp, " & ");
                    cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);
                    and = false;
                }

                item = node_list->val;
                if (item->type == CELL_NODE) {
                    cstr_printf(&str_tmp, "_N[%d]", item->index);
                    cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);
                }

                if (item->type == CELL_STR) {
                    cstr_printf(&str_tmp, "%s%s", item->neg ? "!" : "", item->str);
                    cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);
                }

                if (item->type == CELL_AND)
                    and = true;
            }

            cstr_printf(&str_tmp, " ) ");
            cstr_append(&((*str_out)[*str_out_qty]), str_tmp.str);

            and = false;
            or = false;
        }

        if (cstr_size((*str_out)[*str_out_qty]) != 0) {
            ++(*str_out_qty);
            *str_out = realloc(*str_out, (*str_out_qty + 1) * sizeof(cstr*));
            (*str_out)[*str_out_qty] = cstr_with_capacity(1);
        }
    }

    cstr_drop(&str_tmp);

#ifdef LADDER_DEBUG
    printf("\nOUT (%d):\n", *str_out_qty);
    for (block = 0; block < *str_out_qty; block++)
        if (cstr_length((*str_out)[block]) > 0)
            printf("%s\n", (*str_out)[block].str);
    printf("end OUT\n");
#endif
}

void ladder_substitute_nodes(int pos1, int pos2, cstr *str_in, cstr *str_out) {
    int line, column, n;
    bool neg = false;
    cstr tmp = cstr_with_capacity(1);
    cstr tmp2 = cstr_with_capacity(1);
    cstr left = cstr_with_capacity(1);
    cstr right = cstr_with_capacity(1);

    for (line = 0; line < pos1; line++) {
        n = cstr_find(str_in[line], "=");

        cstr_assign_n(&left, str_in[line].str, n - 1);
        cstr_assign(&right, str_in[line].str + n + 2);

        // if used by out not substitute
        for (column = 0; column < pos2; column++) {
            int find = cstr_find(str_out[column], left.str);
            if (find != -1) {
                cstr_clear(&left);
                cstr_clear(&right);
                neg = true;
                break;
            }
        }

        if (neg) {
            neg = false;
            continue;
        }

        cstr_assign(&tmp, "(");
        cstr_assign(&tmp2, ")");
        cstr_append(&tmp, right.str);
        cstr_append(&tmp, tmp2.str);

        for (column = 0; column < pos1; column++) {
            if (column == line)
                continue;
            cstr_replace_all(&str_in[column], left.str, tmp.str);
        }

        cstr_clear(&str_in[line]);
        cstr_clear(&left);
        cstr_clear(&right);

    }
    cstr_drop(&left);
    cstr_drop(&right);
    cstr_drop(&tmp);
    cstr_drop(&tmp2);
}

void ladder_parse(cstr **result_in, cstr **result_out, int *result_in_qty, int *result_out_qty, rung_t **rung) {
    int n, pos, str_in_qty, str_out_qty, blocks_qty, nodes_qty;
    cstr *str_in, *str_out;
    cell_t **blocks_tmp;
    cell_t **lns;
    list_t **blocks_list;

    lns = malloc((*rung)->line_qty * sizeof(cell_t*));
    for (n = 0; n < (*rung)->line_qty; n++)
        lns[n] = calloc(cstr_length((*rung)->line[n]), sizeof(cell_t));

    ladder_identify_char(rung, &lns);
    nodes_qty = ladder_unify_nodes(rung, &lns);
    blocks_qty = ladder_separate_blocks(&blocks_tmp, rung, nodes_qty, &lns);

    for (n = 0; n < (*rung)->line_qty; n++)
        free(lns[n]);
    free(lns);

    blocks_qty = ladder_allocate_list_blocks(&blocks_tmp, rung, &blocks_list, blocks_qty);

    str_in = malloc(sizeof(cstr*));
    str_out = malloc(sizeof(cstr*));

    ladder_join_nodes(&blocks_list, nodes_qty, blocks_qty, &str_in, &str_out, &str_in_qty, &str_out_qty);

    for (n = 0; n < blocks_qty; n++) {
        free(blocks_tmp[n]);
    }
    free(blocks_tmp);

    for (n = 0; n < blocks_qty; n++) {
        free(blocks_list[n]);
    }
    free(blocks_list);

    ladder_substitute_nodes(str_in_qty, str_out_qty, str_in, str_out);

    for (n = 0; n < str_in_qty; n++) {
        while ((pos = cstr_find(str_in[n], "( ")) > 0)
            cstr_erase(&str_in[n], pos + 1);
        while ((pos = cstr_find(str_in[n], " )")) > 0)
            cstr_erase(&str_in[n], pos);
    }
    for (n = 0; n < str_out_qty; n++) {
        while ((pos = cstr_find(str_out[n], "( ")) > 0)
            cstr_erase(&str_out[n], pos + 1);
        while ((pos = cstr_find(str_out[n], " )")) > 0)
            cstr_erase(&str_out[n], pos);
    }

    // final result
    *result_in = str_in;
    *result_out = str_out;
    *result_in_qty = str_in_qty;
    *result_out_qty = str_out_qty;
}
