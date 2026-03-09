/*
** EPITECH PROJECT, 2025
** str_low
** File description:
** str_low
*/

void str_low(char *restrict __str)
{
    const int difference = 'a' - 'A';

    for (int i = 0; __str[i]; i++) {
        if (__str[i] >= 'A' && __str[i] <= 'Z')
            __str[i] += difference;
    }
}
