/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static float get_val(dataframe_t *dataframe, int col, int row)
{
    column_type_t type = dataframe->columns[col].type;

    if (type == INT)
        return (float)dataframe->columns[col].data[row].i;
    if (type == UINT)
        return (float)dataframe->columns[col].data[row].ui;
    return dataframe->columns[col].data[row].f;
}

static float get_stats(dataframe_t *dataframe, int col,
    float *min, float *max)
{
    float sum = 0;
    int count = dataframe->nb_rows;
    float val = 0;

    *min = get_val(dataframe, col, 0);
    *max = get_val(dataframe, col, 0);
    for (int i = 0; i < count; i++) {
        val = get_val(dataframe, col, i);
        sum += val;
        if (val < *min)
            *min = val;
        if (val > *max)
            *max = val;
    }
    return sum / count;
}

static void describe_column(dataframe_t *dataframe, int col)
{
    column_type_t type = dataframe->columns[col].type;
    float min = 0;
    float max = 0;
    float mean = 0;
    float std_sum = 0;
    float diff = 0;

    if (type != INT && type != UINT && type != FLOAT)
        return;
    mean = get_stats(dataframe, col, &min, &max);
    for (int i = 0; i < dataframe->nb_rows; i++) {
        diff = get_val(dataframe, col, i) - mean;
        std_sum += diff * diff;
    }
    printf(DF_DESCRIBE,
        dataframe->columns[col].name, dataframe->nb_rows,
        mean, sqrt(std_sum / dataframe->nb_rows), min, max);
}

void df_describe(dataframe_t *dataframe)
{
    for (int i = 0; i < dataframe->nb_columns; i++)
        describe_column(dataframe, i);
}
