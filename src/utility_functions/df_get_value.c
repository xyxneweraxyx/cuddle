/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

void *df_get_value(dataframe_t *dataframe, int row, const char *column)
{
    if (!dataframe || !column || row >= dataframe->nb_rows)
        return NULL;
    for (int i = 0; i < dataframe->nb_columns; i++) {
        if (!str_cmp(column, dataframe->columns[i].name))
            return (void *)&(dataframe->columns[i].data[row]);
    }
    return NULL;
}
