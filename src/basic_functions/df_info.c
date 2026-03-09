/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static const char *type_to_str(column_type_t type)
{
    if (type == BOOL)
        return "bool";
    if (type == INT)
        return "int";
    if (type == UINT)
        return "unsigned int";
    if (type == FLOAT)
        return "float";
    if (type == STRING)
        return "string";
    return "undefined";
}

void df_info(dataframe_t *dataframe)
{
    printf("%d columns:\n", dataframe->nb_columns);
    for (int i = 0; i < dataframe->nb_columns; i++)
        printf("- %s: %s\n", dataframe->columns[i].name,
            type_to_str(dataframe->columns[i].type));
}
