/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static void **verify_alloc(dataframe_t *dataframe, const char *column)
{
    void **values = NULL;

    if (!dataframe || !column || dataframe->nb_rows == 0)
        return NULL;
    values = malloc(sizeof(void *) * dataframe->nb_rows + 1);
    if (!values)
        return NULL;
    for (int i = 0; i <= dataframe->nb_rows; i++)
        values[i] = NULL;
    return values;
}

void **df_get_values(dataframe_t *dataframe, const char *column)
{
    void **values = verify_alloc(dataframe, column);
    int column_num = 0;

    if (!values)
        return NULL;
    for (; column_num < dataframe->nb_columns &&
        str_cmp(column, dataframe->columns[column_num].name); column_num++);
    if (column_num == dataframe->nb_columns) {
        free(values);
        return NULL;
    }
    for (int i = 0; i < dataframe->nb_rows; i++) {
        if (dataframe->columns[column_num].type == STRING)
            values[i] = (void *)dataframe->columns[column_num].data[i].str;
        else
            values[i] = (void *)&(dataframe->columns[column_num].data[i]);
    }
    return values;
}
