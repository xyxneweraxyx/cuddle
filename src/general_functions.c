/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../include/dataframe.h"

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

int copy_all_data(dataframe_t *old_df, dataframe_t *new_df)
{
    for (int i = 0; i < new_df->nb_columns; i++) {
        new_df->columns[i].data = c_alloc(sizeof(data_t),
            new_df->nb_rows, new_df->alloc);
        if (!new_df->columns[i].data)
            return DF_FAIL;
        for (int j = 0; j < new_df->nb_rows; j++) {
            if (old_df->columns[i].type != STRING) {
                new_df->columns[i].data[j] = old_df->columns[i].data[j];
                continue;
            }
            new_df->columns[i].data[j].str = c_alloc(sizeof(char),
                str_len(old_df->columns[i].data[j].str) + 1, new_df->alloc);
            if (!new_df->columns[i].data[j].str)
                return DF_FAIL;
            str_cpy(old_df->columns[i].data[j].str,
                new_df->columns[i].data[j].str);
        }
    }
    return DF_SUCC;
}

dataframe_t *free_return(bool success,
    dataframe_t *dataframe, c_alloc_t *alloc_to_free)
{
    if (success) {
        if (alloc_to_free)
            c_delete(alloc_to_free, true);
        return dataframe;
    }
    if (alloc_to_free)
        c_delete(alloc_to_free, true);
    if (dataframe && dataframe->alloc)
        c_delete(dataframe->alloc, true);
    return NULL;
}