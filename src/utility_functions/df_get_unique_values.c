/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static bool is_value_unique(dataframe_t *dataframe, int col, int row)
{
    column_type_t type = dataframe->columns[col].type;
    data_t *data = dataframe->columns[col].data;

    for (int i = 0; i < row; i++) {
        if (type == BOOL && data[i].b == data[row].b)
            return false;
        if (type == UINT && data[i].ui == data[row].ui)
            return false;
        if (type == INT && data[i].i == data[row].i)
            return false;
        if (type == FLOAT && data[i].f == data[row].f)
            return false;
        if (type == STRING && !str_cmp(data[i].str, data[row].str))
            return false;
    }
    return true;
}

static int write_data(dataframe_t *dataframe, int col, void **values)
{
    int written = 0;
    column_type_t type = dataframe->columns[col].type;
    data_t *data = dataframe->columns[col].data;

    for (int i = 0; i < dataframe->nb_rows; i++) {
        if (!is_value_unique(dataframe, col, i))
            continue;
        if (type == STRING)
            values[written] = (void *)data[i].str;
        else
            values[written] = (void *)&data[i];
        written++;
    }
    return DF_SUCC;
}

void **df_get_unique_values(dataframe_t *dataframe, const char *column)
{
    void **values = NULL;
    int column_num = 0;

    if (!dataframe || !column || dataframe->nb_rows == 0)
        return NULL;
    values = malloc(sizeof(void *) * dataframe->nb_rows);
    if (!values)
        return NULL;
    for (int i = 0; i < dataframe->nb_rows; i++)
        values[i] = NULL;
    for (; column_num < dataframe->nb_columns &&
        str_cmp(column, dataframe->columns[column_num].name); column_num++);
    if (column_num == dataframe->nb_columns ||
        write_data(dataframe, column_num, values) == DF_FAIL) {
        free(values);
        return NULL;
    }
    return values;
}
