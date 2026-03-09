/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static int copy_data(dataframe_t *old_df, dataframe_t *new_df, int nb_rows)
{
    new_df->nb_columns = old_df->nb_columns;
    new_df->nb_rows = (nb_rows > old_df->nb_rows) ? old_df->nb_rows : nb_rows;
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

static int write_single_data(dataframe_t *old_df,
    dataframe_t *new_df, int row, int col)
{
    column_type_t type = old_df->columns[col].type;
    data_t *old_data = &(old_df->columns[col].data[row]);
    data_t *new_data = &(new_df->columns[col].data[row]);

    if (type == BOOL)
        new_data->b = old_data->b;
    if (type == INT)
        new_data->i = old_data->i;
    if (type == UINT)
        new_data->ui = old_data->ui;
    if (type == FLOAT)
        new_data->f = old_data->f;
    if (type == STRING) {
        new_data->str = c_alloc(sizeof(char),
            str_len(old_data->str) + 1, new_df->alloc);
        if (!new_data->str)
            return DF_FAIL;
        str_cpy(old_data->str, new_data->str);
    }
    return DF_SUCC;
}

static int copy_n_first(dataframe_t *old_df,
    dataframe_t *new_df, int nb_rows)
{
    bool failed = false;

    for (int i = 0; i < new_df->nb_columns; i++) {
        new_df->columns[i].data = c_alloc(sizeof(data_t),
            nb_rows, new_df->alloc);
        if (!new_df->columns[i].data)
            return DF_FAIL;
        for (int j = 0; j < new_df->nb_rows; j++)
            failed = (write_single_data(old_df, new_df, j, i) ==
                DF_FAIL) ? true : failed;
        if (failed)
            break;
    }
    return (failed) ? DF_FAIL : DF_SUCC;
}

dataframe_t *df_head(dataframe_t *dataframe, int nb_rows)
{
    c_alloc_t *new_alloc = c_ini((uint16_t)50);
    dataframe_t *new_df = NULL;

    if (!new_alloc)
        return free_return(false, NULL, NULL);
    new_df = c_alloc(sizeof(dataframe_t), 1, new_alloc);
    if (!new_df)
        return free_return(false, NULL, new_alloc);
    new_df->alloc = new_alloc;
    if (copy_data(dataframe, new_df, nb_rows) == DF_FAIL)
        return free_return(false, new_df, new_alloc);
    if (copy_n_first(dataframe, new_df, nb_rows) == DF_FAIL)
        return free_return(false, new_df, new_alloc);
    return new_df;
}
