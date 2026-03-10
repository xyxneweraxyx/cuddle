/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static void store_result(groupby_t *gb,
    int row, void *result, column_type_t type)
{
    int col = gb->new_col;

    if (type == BOOL)
        gb->new_df->columns[col].data[row].b = *(bool *)result;
    if (type == INT)
        gb->new_df->columns[col].data[row].i = *(int *)result;
    if (type == UINT)
        gb->new_df->columns[col].data[row].ui = *(unsigned int *)result;
    if (type == FLOAT)
        gb->new_df->columns[col].data[row].f = *(float *)result;
    if (type == STRING)
        gb->new_df->columns[col].data[row].str = *(char **)result;
}

static int process_group(groupby_t *gb,
    int agg_col, int row, int dest)
{
    void **values = collect_group(gb, row, agg_col);
    void *result = NULL;
    column_type_t type = gb->old_df->columns[agg_col].type;

    if (!values)
        return DF_FAIL;
    result = gb->agg_func(values, count_in_group(gb, row));
    free(values);
    if (!result)
        return DF_FAIL;
    store_result(gb, dest, result, type);
    free(result);
    return DF_SUCC;
}

static int fill_one_agg(groupby_t *gb, int agg_col)
{
    int dest = 0;

    for (int i = 0; i < gb->old_df->nb_rows; i++) {
        if (!is_first_occ(gb->old_df, gb->group_col, i))
            continue;
        if (process_group(gb, agg_col, i, dest) == DF_FAIL)
            return DF_FAIL;
        dest++;
    }
    return DF_SUCC;
}

static int setup_col(dataframe_t *new_df, int new_col,
    const char *name, column_type_t type)
{
    new_df->columns[new_col].type = type;
    new_df->columns[new_col].name = c_alloc(sizeof(char),
        str_len(name) + 1, new_df->alloc);
    if (!new_df->columns[new_col].name)
        return DF_FAIL;
    str_cpy(name, new_df->columns[new_col].name);
    new_df->columns[new_col].data = c_alloc(sizeof(data_t),
        new_df->nb_rows, new_df->alloc);
    if (!new_df->columns[new_col].data)
        return DF_FAIL;
    return DF_SUCC;
}

static int setup_result(groupby_t *gb, const char **to_agg)
{
    int nb_agg = 0;
    int col = gb->group_col;
    int agg_col;

    for (; to_agg[nb_agg]; nb_agg++);
    gb->new_df->nb_columns = nb_agg + 1;
    gb->new_df->columns = c_alloc(sizeof(column_t),
        gb->new_df->nb_columns, gb->new_df->alloc);
    if (!gb->new_df->columns)
        return DF_FAIL;
    if (setup_col(gb->new_df, 0, gb->old_df->columns[col].name,
        gb->old_df->columns[col].type) == DF_FAIL)
        return DF_FAIL;
    for (int i = 0; to_agg[i]; i++) {
        agg_col = find_col(gb->old_df, to_agg[i]);
        if (agg_col == -1)
            return DF_FAIL;
        if (setup_col(gb->new_df, i + 1, to_agg[i],
            gb->old_df->columns[agg_col].type) == DF_FAIL)
            return DF_FAIL;
    }
    return DF_SUCC;
}

static int copy_cell(dataframe_t *new_df, int col,
    int row, data_t src, column_type_t type)
{
    if (type != STRING) {
        new_df->columns[col].data[row] = src;
        return DF_SUCC;
    }
    new_df->columns[col].data[row].str = c_alloc(sizeof(char),
        str_len(src.str) + 1, new_df->alloc);
    if (!new_df->columns[col].data[row].str)
        return DF_FAIL;
    str_cpy(src.str, new_df->columns[col].data[row].str);
    return DF_SUCC;
}

static int fill_group_col(groupby_t *gb)
{
    int dest = 0;
    int col = gb->group_col;
    column_type_t type = gb->old_df->columns[col].type;
    data_t src;

    for (int i = 0; i < gb->old_df->nb_rows; i++) {
        if (!is_first_occ(gb->old_df, col, i))
            continue;
        src = gb->old_df->columns[col].data[i];
        if (copy_cell(gb->new_df, 0, dest, src, type) == DF_FAIL)
            return DF_FAIL;
        dest++;
    }
    return DF_SUCC;
}

static int fill_agg_cols(groupby_t *gb, const char **to_agg)
{
    int agg_col;

    for (int j = 0; to_agg[j]; j++) {
        agg_col = find_col(gb->old_df, to_agg[j]);
        if (agg_col == -1)
            return DF_FAIL;
        gb->new_col = j + 1;
        if (fill_one_agg(gb, agg_col) == DF_FAIL)
            return DF_FAIL;
    }
    return DF_SUCC;
}

static dataframe_t *init_new_df(groupby_t *gb)
{
    c_alloc_t *alloc = c_ini((uint16_t)50);
    dataframe_t *new_df = NULL;

    if (!alloc)
        return NULL;
    new_df = c_alloc(sizeof(dataframe_t), 1, alloc);
    if (!new_df)
        return free_return(false, NULL, alloc);
    new_df->alloc = alloc;
    new_df->sep = c_alloc(sizeof(char),
        str_len(gb->old_df->sep) + 1, alloc);
    if (!new_df->sep)
        return free_return(false, new_df, NULL);
    str_cpy(gb->old_df->sep, new_df->sep);
    for (int i = 0; i < gb->old_df->nb_rows; i++)
        if (is_first_occ(gb->old_df, gb->group_col, i))
            new_df->nb_rows++;
    return new_df;
}

dataframe_t *df_groupby(dataframe_t *dataframe,
    const char *aggregate_by, const char **to_aggregate,
    void *(*agg_func)(void **values, int nb_values))
{
    groupby_t gb = {0};

    if (!dataframe || !aggregate_by || !to_aggregate || !agg_func)
        return NULL;
    gb.old_df = dataframe;
    gb.agg_func = agg_func;
    gb.group_col = find_col(dataframe, aggregate_by);
    if (gb.group_col == -1)
        return NULL;
    gb.new_df = init_new_df(&gb);
    if (!gb.new_df)
        return NULL;
    if (setup_result(&gb, to_aggregate) == DF_FAIL ||
        fill_group_col(&gb) == DF_FAIL ||
        fill_agg_cols(&gb, to_aggregate) == DF_FAIL)
        return free_return(false, gb.new_df, NULL);
    return gb.new_df;
}