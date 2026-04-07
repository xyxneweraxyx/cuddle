/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int dfp_is_conversion_correct(column_type_t from, column_type_t to)
{
    if (from == to)
        return DF_FAIL;
    return DF_SUCC;
}

static void convert_to_str(data_t *cell,
    column_type_t from, c_alloc_t *alloc)
{
    char buf[64];

    if (from == INT)
        snprintf(buf, sizeof(buf), "%d", cell->i);
    if (from == UINT)
        snprintf(buf, sizeof(buf), "%u", cell->ui);
    if (from == FLOAT)
        snprintf(buf, sizeof(buf), "%f", cell->f);
    if (from == BOOL)
        snprintf(buf, sizeof(buf), "%s", cell->b ? "true" : "false");
    cell->str = c_alloc(sizeof(char), str_len(buf) + 1, alloc);
    if (cell->str)
        str_cpy(buf, cell->str);
}

static void convert_from_str(data_t *cell, column_type_t to)
{
    if (to == INT)
        cell->i = atoi(cell->str);
    if (to == UINT)
        cell->ui = (unsigned int)atoi(cell->str);
    if (to == FLOAT)
        cell->f = atof(cell->str);
    if (to == BOOL) {
        if (!str_cmp("true", cell->str))
            cell->b = true;
        else
            cell->b = false;
    }
}

static void convert_to_float(data_t *cell, column_type_t from)
{
    if (from == INT)
        cell->f = (float)cell->i;
    if (from == UINT)
        cell->f = (float)cell->ui;
    if (from == BOOL)
        cell->f = (float)cell->b;
}

static void convert_to_int(data_t *cell, column_type_t from)
{
    if (from == FLOAT)
        cell->i = (int)cell->f;
    if (from == UINT)
        cell->i = (int)cell->ui;
    if (from == BOOL)
        cell->i = (int)cell->b;
}

static void convert_to_uint(data_t *cell, column_type_t from)
{
    if (from == FLOAT)
        cell->ui = (unsigned int)cell->f;
    if (from == INT)
        cell->ui = (unsigned int)cell->i;
    if (from == BOOL)
        cell->ui = (unsigned int)cell->b;
}

static void convert_to_bool(data_t *cell, column_type_t from)
{
    if (from == FLOAT)
        cell->b = (cell->f == 0) ? false : true;
    if (from == INT)
        cell->b = (cell->i == 0) ? false : true;
    if (from == UINT)
        cell->b = (cell->ui == 0) ? false : true;
}

static void convert_cell(data_t *cell,
    column_type_t from, column_type_t to, c_alloc_t *alloc)
{
    if (to == STRING)
        return convert_to_str(cell, from, alloc);
    if (from == STRING)
        return convert_from_str(cell, to);
    if (to == FLOAT)
        return convert_to_float(cell, from);
    if (to == INT)
        return convert_to_int(cell, from);
    if (to == UINT)
        return convert_to_uint(cell, from);
    if (to == BOOL)
        return convert_to_bool(cell, from);
}

static dataframe_t *build_converted(dataframe_t *dataframe,
    int col, column_type_t downcast)
{
    dataframe_t *new_df = new_dataframe(dataframe);
    column_type_t from = dataframe->columns[col].type;

    if (!new_df)
        return NULL;
    new_df->nb_rows = dataframe->nb_rows;
    if (copy_all_data(dataframe, new_df) == DF_FAIL) {
        c_delete(new_df->alloc, true);
        return NULL;
    }
    new_df->columns[col].type = downcast;
    for (int i = 0; i < new_df->nb_rows; i++)
        convert_cell(&new_df->columns[col].data[i], from,
            downcast, new_df->alloc);
    return new_df;
}

dataframe_t *df_to_type(dataframe_t *dataframe,
    const char *column, column_type_t downcast)
{
    int col = 0;

    if (!dataframe || !column)
        return NULL;
    for (; col < dataframe->nb_columns &&
        str_cmp(column, dataframe->columns[col].name); col++);
    if (col == dataframe->nb_columns)
        return NULL;
    if (dfp_is_conversion_correct(dataframe->columns[col].type,
            downcast) == DF_FAIL)
        return NULL;
    return build_converted(dataframe, col, downcast);
}
