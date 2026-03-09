/*
** EPITECH PROJECT, 2025
** str_upp
** File description:
** str_upp
*/

void str_upp(char *restrict __str)
{
    const int difference = 'a' - 'A';

    for (int i = 0; __str[i]; i++) {
        if (__str[i] >= 'a' && __str[i] <= 'z')
            __str[i] -= difference;
    }
}
