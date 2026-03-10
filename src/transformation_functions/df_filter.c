/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int count_passing(dataframe_t *dataframe,
    int col, bool (*filter_func)(void *))
{
    int count = 0;

    for (int i = 0; i < dataframe->nb_rows; i++)
        if (filter_func((void *)&(dataframe->columns[col].data[i])))
            count++;
    return count;
}

static int copy_row(dataframe_t *old_df, dataframe_t *new_df,
    int old_row, int new_row)
{
    for (int j = 0; j < new_df->nb_columns; j++) {
        if (old_df->columns[j].type != STRING) {
            new_df->columns[j].data[new_row] = old_df->columns[j].data[old_row];
            continue;
        }
        new_df->columns[j].data[new_row].str = c_alloc(sizeof(char),
            str_len(old_df->columns[j].data[old_row].str) + 1, new_df->alloc);
        if (!new_df->columns[j].data[new_row].str)
            return DF_FAIL;
        str_cpy(old_df->columns[j].data[old_row].str,
            new_df->columns[j].data[new_row].str);
    }
    return DF_SUCC;
}

static int fill_filtered(dataframe_t *old_df, dataframe_t *new_df,
    int col, bool (*filter_func)(void *))
{
    int new_row = 0;

    for (int i = 0; i < old_df->nb_rows; i++) {
        if (!filter_func((void *)&(old_df->columns[col].data[i])))
            continue;
        if (copy_row(old_df, new_df, i, new_row) == DF_FAIL)
            return DF_FAIL;
        new_row++;
    }
    return DF_SUCC;
}

static int alloc_columns(dataframe_t *new_df, int nb_rows)
{
    for (int i = 0; i < new_df->nb_columns; i++) {
        new_df->columns[i].data = c_alloc(sizeof(data_t),
            nb_rows, new_df->alloc);
        if (!new_df->columns[i].data)
            return DF_FAIL;
    }
    return DF_SUCC;
}

static dataframe_t *build_filtered(dataframe_t *dataframe,
    int col, int nb_rows, bool (*filter_func)(void *))
{
    dataframe_t *new_df = new_dataframe(dataframe);

    if (!new_df)
        return NULL;
    new_df->nb_rows = nb_rows;
    if (alloc_columns(new_df, nb_rows) == DF_FAIL) {
        c_delete(new_df->alloc, true);
        return NULL;
    }
    if (fill_filtered(dataframe, new_df, col, filter_func) == DF_FAIL) {
        c_delete(new_df->alloc, true);
        return NULL;
    }
    return new_df;
}

dataframe_t *df_filter(dataframe_t *dataframe,
    const char *column, bool (*filter_func)(void *value))
{
    int col = 0;
    int nb_rows = 0;

    if (!dataframe || !column || !filter_func)
        return NULL;
    for (; col < dataframe->nb_columns &&
        str_cmp(column, dataframe->columns[col].name); col++);
    if (col == dataframe->nb_columns)
        return NULL;
    nb_rows = count_passing(dataframe, col, filter_func);
    return (nb_rows == 0) ? NULL : build_filtered(dataframe, col,
        nb_rows, filter_func);
}
