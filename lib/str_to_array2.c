/*
** EPITECH PROJECT, 2025
** str_to_array
** File description:
** Convert a string to an array based on a number of seps and a max amount.
*/

#include "./../include/dataframe.h"

int match_sep(const char *str, const char **seps)
{
    int len;
    int i = 0;

    for (i = 0; seps[i]; i++) {
        len = (int)strlen(seps[i]);
        if (strncmp(str, seps[i], len) == 0)
            return len;
    }
    return 0;
}

void skip_seps(parse_state_t *ps)
{
    int sep_len = match_sep(ps->str + ps->cur, ps->sep);

    while (sep_len > 0) {
        ps->cur += sep_len;
        sep_len = match_sep(ps->str + ps->cur, ps->sep);
    }
}

void skip_one_word(parse_state_t *ps)
{
    int sep_len = match_sep(ps->str + ps->cur, ps->sep);

    while (ps->str[ps->cur] && sep_len == 0) {
        ps->cur++;
        sep_len = match_sep(ps->str + ps->cur, ps->sep);
    }
}
