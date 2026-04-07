/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

int find_col(dataframe_t *df, const char *name)
{
    for (int i = 0; i < df->nb_columns; i++)
        if (!str_cmp(name, df->columns[i].name))
            return i;
    return -1;
}

bool data_eq(data_t a, data_t b, column_type_t type)
{
    if (type == BOOL)
        return a.b == b.b;
    if (type == INT)
        return a.i == b.i;
    if (type == UINT)
        return a.ui == b.ui;
    if (type == FLOAT)
        return a.f == b.f;
    if (type == STRING)
        return !str_cmp(a.str, b.str);
    return false;
}

bool is_first_occ(dataframe_t *df, int col, int row)
{
    column_type_t type = df->columns[col].type;

    for (int i = 0; i < row; i++)
        if (data_eq(df->columns[col].data[i],
                df->columns[col].data[row], type))
            return false;
    return true;
}

int count_in_group(groupby_t *gb, int ref_row)
{
    int n = 0;
    data_t ref = gb->old_df->columns[gb->group_col].data[ref_row];
    column_type_t type = gb->old_df->columns[gb->group_col].type;

    for (int i = 0; i < gb->old_df->nb_rows; i++)
        if (data_eq(gb->old_df->columns[gb->group_col].data[i],
                ref, type))
            n++;
    return n;
}

void **collect_group(groupby_t *gb, int ref_row, int agg_col)
{
    int nb = count_in_group(gb, ref_row);
    data_t ref = gb->old_df->columns[gb->group_col].data[ref_row];
    column_type_t type = gb->old_df->columns[gb->group_col].type;
    void **values = malloc(sizeof(void *) * nb);
    int w = 0;

    if (!values)
        return NULL;
    for (int i = 0; i < gb->old_df->nb_rows; i++) {
        if (data_eq(gb->old_df->columns[gb->group_col].data[i],
                ref, type)) {
            values[w] = (void *)&gb->old_df->columns[agg_col].data[i];
            w++;
        }
    }
    return values;
}
