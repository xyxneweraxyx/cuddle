/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static void write_single_val(dataframe_t *dataframe,
    FILE *file, int row, int col)
{
    column_type_t type = dataframe->columns[col].type;
    data_t data = dataframe->columns[col].data[row];

    if (type == BOOL && data.b == true)
        fprintf(file, "true");
    if (type == BOOL && data.b == false)
        fprintf(file, "false");
    if (type == INT)
        fprintf(file, "%i", data.i);
    if (type == UINT)
        fprintf(file, "%u", data.ui);
    if (type == FLOAT)
        fprintf(file, "%f", data.f);
    if (type == STRING)
        fprintf(file, "%s", data.str);
    if (col != dataframe->nb_columns - 1)
        fprintf(file, "%s", dataframe->sep);
}

int df_write_csv(dataframe_t *dataframe, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (!file)
        return DF_FAIL;
    for (int i = 0; i < dataframe->nb_columns; i++) {
        fprintf(file, "%s", dataframe->columns[i].name);
        if (i != dataframe->nb_columns - 1)
            fprintf(file, "%s", dataframe->sep);
    }
    fprintf(file, "\n");
    for (int i = 0; i < dataframe->nb_rows; i++) {
        for (int j = 0; j < dataframe->nb_columns; j++)
            write_single_val(dataframe, file, i, j);
        fprintf(file, "\n");
    }
    fclose(file);
    return DF_SUCC;
}
