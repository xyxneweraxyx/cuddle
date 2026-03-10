/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int dfp_is_conversion_correct(column_type_t from, column_type_t to)
{
    if (from == STRING)
        return DF_FAIL;
    if (from == FLOAT && (to == INT || to == UINT || to == BOOL))
        return DF_FAIL;
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

static void convert_cell(data_t *cell,
    column_type_t from, column_type_t to, c_alloc_t *alloc)
{
    if (to == STRING) {
        convert_to_str(cell, from, alloc);
        return;
    }
    if (to == FLOAT)
        cell->f = (from == INT) ? (float)cell->i : (float)cell->ui;
    if (to == INT)
        cell->i = (int)cell->ui;
    if (to == UINT)
        cell->ui = (unsigned int)cell->i;
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
