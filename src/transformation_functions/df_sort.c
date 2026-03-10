/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int dfp_invert_rows(dataframe_t *dataframe, int row1, int row2)
{
    data_t tmp;

    for (int i = 0; i < dataframe->nb_columns; i++) {
        tmp = dataframe->columns[i].data[row1];
        dataframe->columns[i].data[row1] = dataframe->columns[i].data[row2];
        dataframe->columns[i].data[row2] = tmp;
    }
    return DF_SUCC;
}

static void bubble_sort(dataframe_t *dataframe,
    int col, bool (*sort_func)(void *, void *))
{
    int n = dataframe->nb_rows;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (!sort_func(
                (void *)&dataframe->columns[col].data[j],
                (void *)&dataframe->columns[col].data[j + 1]))
                dfp_invert_rows(dataframe, j, j + 1);
        }
    }
}

dataframe_t *df_sort(dataframe_t *dataframe,
    const char *column, bool (*sort_func)(void *value1, void *value2))
{
    int col = 0;
    dataframe_t *new_df = NULL;

    if (!dataframe || !column || !sort_func)
        return NULL;
    for (; col < dataframe->nb_columns &&
        str_cmp(column, dataframe->columns[col].name); col++);
    if (col == dataframe->nb_columns)
        return NULL;
    new_df = new_dataframe(dataframe);
    if (!new_df)
        return NULL;
    new_df->nb_rows = dataframe->nb_rows;
    if (copy_all_data(dataframe, new_df) == DF_FAIL) {
        c_delete(new_df->alloc, true);
        return NULL;
    }
    bubble_sort(new_df, col, sort_func);
    return new_df;
}