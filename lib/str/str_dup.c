/*
** EPITECH PROJECT, 2025
** str_dup
** File description:
** str_dup
*/

#include <stdlib.h>

static int str_len(const char *restrict str)
{
    int i = 0;

    for (; str[i]; i++);
    return i;
}

static void str_cpy(const char *restrict src, char *restrict dest)
{
    int i = 0;

    for (; src[i]; i++)
        dest[i] = src[i];
    dest[i] = 0;
}

char *str_dup(const char *restrict __str)
{
    int len = str_len(__str);
    char *alloc = malloc(sizeof(char) * (len + 1));

    if (!alloc)
        return NULL;
    str_cpy(__str, alloc);
    alloc[len] = 0;
    return alloc;
}
