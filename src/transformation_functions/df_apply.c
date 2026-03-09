/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int copy_data(dataframe_t *old_df, dataframe_t *new_df)
{
    new_df->nb_columns = old_df->nb_columns;
    new_df->nb_rows = old_df->nb_rows;
    new_df->sep = c_alloc(sizeof(char),
        str_len(old_df->sep) + 1, new_df->alloc);
    if (!new_df->sep)
        return DF_FAIL;
    str_cpy(old_df->sep, new_df->sep);
    new_df->columns = c_alloc(sizeof(column_t),
        new_df->nb_columns, new_df->alloc);
    if (!new_df->columns)
        return DF_FAIL;
    for (int i = 0; i < new_df->nb_columns; i++) {
        new_df->columns[i].type = old_df->columns[i].type;
        new_df->columns[i].name = c_alloc(sizeof(char),
            str_len(old_df->columns[i].name) + 1, new_df->alloc);
        if (!new_df->columns[i].name)
            return DF_FAIL;
        str_cpy(old_df->columns[i].name, new_df->columns[i].name);
    }
    return DF_SUCC;
}

dataframe_t *new_dataframe(dataframe_t *old_df)
{
    c_alloc_t *alloc = NULL;
    dataframe_t *new_df = NULL;

    if (!old_df)
        return NULL;
    alloc = c_ini((uint16_t)50);
    if (!alloc)
        return NULL;
    new_df = c_alloc(sizeof(dataframe_t), 1, alloc);
    if (!new_df) {
        c_delete(alloc, true);
        return NULL;
    }
    new_df->alloc = alloc;
    if (copy_data(old_df, new_df) == DF_FAIL) {
        c_delete(alloc, true);
        return NULL;
    }
    return new_df;
}

static int fill_columns(dataframe_t *old_df, dataframe_t *new_df,
    int column_num, void *(*apply_func)(void *value))
{
    for (int i = 0; i < new_df->nb_columns; i++) {
        new_df->columns[i].data = c_alloc(sizeof(data_t), new_df->nb_rows, new_df->alloc);
        if (!new_df->columns[i].data)
            return DF_FAIL;
        for (int j = 0; j < new_df->nb_rows; i++)
            new_df->columns[i].data[j].str = (i == column_num) ?
                apply_func((void *)&(old_df->columns[i].data[j].str)) :
                old_df->columns[i].data[j].str;
    }
    return DF_SUCC;
}

// union.i = 4000;
// --> 101001000000000
// union.b = false;
// --> 000000000000000

dataframe_t *df_apply(dataframe_t *dataframe,
    const char *column, void *(*apply_func)(void *value))
{
    int column_num = 0;
    dataframe_t *new_df = NULL;
    
    if (!dataframe || !column || dataframe->nb_rows == 0)
        return NULL;
    for (; column_num < dataframe->nb_columns &&
        !str_cmp(column, dataframe->columns[column_num].name); column_num++);
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