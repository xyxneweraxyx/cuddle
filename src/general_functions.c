/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../include/dataframe.h"

static void print_single_data(int col, int row,
    column_type_t type, dataframe_t *dataframe)
{
    if (type == BOOL)
        printf("%d %d BOOL [%d]\n", col, row,
            (int)dataframe->columns[col].data[row].b);
    if (type == INT)
        printf("%d %d INT [%d]\n", col, row,
            dataframe->columns[col].data[row].i);
    if (type == UINT)
        printf("%d %d UINT [%u]\n", col, row,
            dataframe->columns[col].data[row].ui);
    if (type == FLOAT)
        printf("%d %d FLOAT [%f]\n", col, row,
            dataframe->columns[col].data[row].f);
    if (type == STRING)
        printf("%d %d STRING [%s]\n", col, row,
            dataframe->columns[col].data[row].str);
}

void print_data(dataframe_t *dataframe)
{
    printf("\nCOLUMN NAMES ----\n");
    for (int i = 0; i < dataframe->nb_columns; i++)
        printf("%d %s\n", i, dataframe->columns[i].name);
    printf("\n");
    for (int i = 0; i < dataframe->nb_columns; i++) {
        printf("COL %d ----\n", i);
        for (int j = 0; j < dataframe->nb_rows; j++)
            print_single_data(i, j, dataframe->columns[i].type, dataframe);
        printf("\n");
    }
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