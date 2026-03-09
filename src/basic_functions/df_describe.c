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

static void get_stats(dataframe_t *dataframe, int col,
    float *min, float *max, float *mean)
{
    float sum = 0;
    int count = dataframe->nb_rows;

    *min = *max = get_val(dataframe, col, 0);
    for (int i = 0; i < count; i++) {
        float val = get_val(dataframe, col, i);
        sum += val;
        if (val < *min)
            *min = val;
        if (val > *max)
            *max = val;
    }
    *mean = sum / count;
}

static void describe_column(dataframe_t *dataframe, int col)
{
    column_type_t type = dataframe->columns[col].type;
    float min, max, mean, std_sum = 0;

    if (type != INT && type != UINT && type != FLOAT)
        return;
    get_stats(dataframe, col, &min, &max, &mean);
    for (int i = 0; i < dataframe->nb_rows; i++) {
        float diff = get_val(dataframe, col, i) - mean;
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
