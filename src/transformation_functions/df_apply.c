/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int apply_column_normal(dataframe_t *old_df, dataframe_t *new_df,
    int col)
{
    data_t *old_data = old_df->columns[col].data;
    data_t *new_data = new_df->columns[col].data;

    for (int j = 0; j < new_df->nb_rows; j++)
        new_data[j] = old_data[j];
    return DF_SUCC;
}

static int apply_column_modify(dataframe_t *old_df, dataframe_t *new_df,
    int col, void *(*apply_func)(void *))
{
    column_type_t type = old_df->columns[col].type;
    data_t *old_data = old_df->columns[col].data;
    data_t *new_data = new_df->columns[col].data;

    for (int j = 0; j < new_df->nb_rows; j++) {
        if (type == BOOL)
            new_data[j].b = *(bool *)apply_func((void *)&old_data[j].b);
        if (type == INT)
            new_data[j].i = *(int *)apply_func((void *)&old_data[j].i);
        if (type == UINT)
            new_data[j].ui = *(unsigned int *)
                apply_func((void *)&old_data[j].ui);
        if (type == FLOAT)
            new_data[j].f = *(float *)apply_func((void *)&old_data[j].f);
        if (type == STRING)
            new_data[j].str = (char *)apply_func((void *)old_data[j].str);
    }
    return DF_SUCC;
}

static int fill_columns(dataframe_t *old_df, dataframe_t *new_df,
    int target, void *(*apply_func)(void *))
{
    for (int i = 0; i < new_df->nb_columns; i++) {
        new_df->columns[i].data = c_alloc(sizeof(data_t),
            new_df->nb_rows, new_df->alloc);
        if (!new_df->columns[i].data)
            return DF_FAIL;
        if (i == target && apply_column_modify(old_df,
                new_df, i, apply_func) == DF_FAIL)
            return DF_FAIL;
        if (i != target && apply_column_normal(old_df,
                new_df, i) == DF_FAIL)
            return DF_FAIL;
    }
    return DF_SUCC;
}

dataframe_t *df_apply(dataframe_t *dataframe,
    const char *column, void *(*apply_func)(void *value))
{
    int column_num = 0;
    dataframe_t *new_df = NULL;

    if (!dataframe || !column || dataframe->nb_rows == 0)
        return NULL;
    for (; column_num < dataframe->nb_columns &&
        str_cmp(column, dataframe->columns[column_num].name); column_num++);
    if (column_num == dataframe->nb_columns)
        return NULL;
    new_df = new_dataframe(dataframe);
    if (!new_df)
        return NULL;
    if (fill_columns(dataframe, new_df, column_num, apply_func) == DF_FAIL) {
        c_delete(new_df->alloc, true);
        return NULL;
    }
    return new_df;
}
