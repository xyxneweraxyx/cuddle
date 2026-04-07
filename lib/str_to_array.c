/*
** EPITECH PROJECT, 2025
** str_to_array
** File description:
** Convert a string to an array based on a number of seps and a max amount.
*/

#include "./../include/dataframe.h"

static int count_words_skip(parse_state_t *ps)
{
    parse_state_t tmp = *ps;
    int count = 0;

    skip_seps(&tmp);
    while (tmp.str[tmp.cur]) {
        skip_one_word(&tmp);
        count++;
        skip_seps(&tmp);
    }
    return count;
}

static int count_words_exact(parse_state_t *ps)
{
    parse_state_t tmp = *ps;
    int count = 1;
    int sep_len;

    if (!tmp.str[tmp.cur])
        return 0;
    while (tmp.str[tmp.cur]) {
        sep_len = match_sep(tmp.str + tmp.cur, tmp.sep);
        if (sep_len > 0) {
            count++;
            tmp.cur += sep_len;
        } else {
            tmp.cur++;
        }
    }
    return count;
}

static int count_words(parse_state_t *ps)
{
    if (ps->skip_empty)
        return count_words_skip(ps);
    return count_words_exact(ps);
}

static int next_word_len(parse_state_t *ps)
{
    int sep_len = match_sep(ps->str + ps->cur, ps->sep);
    int i = ps->cur;
    int len = 0;

    while (ps->str[i] && sep_len == 0) {
        len++;
        i++;
        sep_len = match_sep(ps->str + i, ps->sep);
    }
    return len;
}

static char *copy_word(parse_state_t *ps, c_alloc_t *alloc)
{
    int len = next_word_len(ps);
    char *word = c_alloc(sizeof(char), len + 1, alloc);
    int sep_len;
    int w = 0;

    if (!word)
        return NULL;
    sep_len = match_sep(ps->str + ps->cur, ps->sep);
    while (ps->str[ps->cur] && sep_len == 0) {
        word[w] = ps->str[ps->cur];
        w++;
        ps->cur++;
        sep_len = match_sep(ps->str + ps->cur, ps->sep);
    }
    word[w] = 0;
    return word;
}

static void fill_skip(parse_state_t *ps, char **result, c_alloc_t *alloc)
{
    int i = 0;

    skip_seps(ps);
    while (ps->str[ps->cur]) {
        result[i] = copy_word(ps, alloc);
        i++;
        skip_seps(ps);
    }
    result[i] = NULL;
}

static void fill_exact(parse_state_t *ps, char **result, c_alloc_t *alloc)
{
    int sep_len;
    int i = 0;

    while (1) {
        result[i] = copy_word(ps, alloc);
        i++;
        sep_len = match_sep(ps->str + ps->cur, ps->sep);
        if (sep_len == 0)
            break;
        ps->cur += sep_len;
    }
    result[i] = NULL;
}

static void fill_result(parse_state_t *ps, char **result, c_alloc_t *alloc)
{
    if (ps->skip_empty)
        fill_skip(ps, result, alloc);
    else
        fill_exact(ps, result, alloc);
}

char **str_to_array(const char *str, const char **sep,
    bool skip_empty, c_alloc_t *alloc)
{
    parse_state_t ps = {str, sep, skip_empty, 0};
    char **result;
    int count;

    if (skip_empty)
        skip_seps(&ps);
    count = count_words(&ps);
    if (count == 0)
        return NULL;
    result = c_alloc(sizeof(char *), count + 1, alloc);
    if (!result)
        return NULL;
    fill_result(&ps, result, alloc);
    return result;
}
