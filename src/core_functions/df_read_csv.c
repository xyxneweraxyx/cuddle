/*
** EPITECH PROJECT, 2025
** cuddle
** File description:
** Function from cuddle lib.
*/

#include "./../../include/dataframe.h"

static column_type_t get_single_type(char *str)
{
    int point_amount = 0;
    int len = str_len(str);
    char lowstr[len + 1];

    str_cpy(str, lowstr);
    str_low(lowstr);
    if (!str_cmp("true", lowstr) || !str_cmp("false", lowstr) || !str[0])
        return (!str[0]) ? STRING : BOOL;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '-' && i == 0 && len > 1)
            continue;
        if (str[i] == '.')
            point_amount++;
        if (point_amount > 1 ||
            ((str[i] < '0' || str[i] > '9') && str[i] != '.'))
            return STRING;
    }
    if (point_amount == 0)
        return (str[0] == '-') ? INT : UINT;
    return (point_amount == 1 && str_len(str) > 1) ? FLOAT : STRING;
}

static int get_type(dataframe_t *dataframe, int col_num,
    char ***lines)
{
    column_type_t real_type = get_single_type(lines[0][col_num]);
    column_type_t found_type = UNDEFINED;

    for (int i = 0; i < dataframe->nb_rows; i++) {
        found_type = get_single_type(lines[i][col_num]);
        if (found_type == real_type)
            continue;
        if ((found_type != BOOL && real_type == BOOL) ||
            found_type == STRING ||
            (found_type == BOOL && real_type != BOOL))
            real_type = STRING;
        if (found_type == INT && real_type == UINT)
            real_type = INT;
        if (found_type == FLOAT && (real_type == UINT || real_type == INT))
            real_type = FLOAT;
    }
    dataframe->columns[col_num].type = real_type;
    return (dataframe->columns[col_num].type == UNDEFINED) ? DF_FAIL : DF_SUCC;
}

static int handle_first_line(dataframe_t *dataframe,
    char *line, c_alloc_t *buff_alloc)
{
    char **values = str_to_array(line,
        (const char *[]){dataframe->sep, NULL}, false, buff_alloc);

    if (!values)
        return DF_FAIL;
    for (; values[dataframe->nb_columns]; dataframe->nb_columns++);
    if (validate_line(dataframe, values) == DF_FAIL)
        return DF_FAIL;
    dataframe->columns = c_alloc(sizeof(column_t),
        dataframe->nb_columns, dataframe->alloc);
    if (!dataframe->columns)
        return DF_FAIL;
    for (int i = 0; values[i]; i++) {
        dataframe->columns[i].name = c_alloc(sizeof(char),
            str_len(values[i]) + 1, dataframe->alloc);
        if (!dataframe->columns[i].name)
            return DF_FAIL;
        str_cpy(values[i], dataframe->columns[i].name);
    }
    return DF_SUCC;
}

static int write_line(char ***lines, int col_num, dataframe_t *dataframe)
{
    for (int i = 0; i < dataframe->nb_rows; i++) {
        if (dataframe->columns[col_num].type == BOOL)
            dataframe->columns[col_num].data[i].b =
                (lines[i][col_num][0] == 't' ||
                lines[i][col_num][0] == 'T') ? true : false;
        if (dataframe->columns[col_num].type == INT)
            dataframe->columns[col_num].data[i].i = atoi(lines[i][col_num]);
        if (dataframe->columns[col_num].type == UINT)
            dataframe->columns[col_num].data[i].ui =
                (unsigned int)atoi(lines[i][col_num]);
        if (dataframe->columns[col_num].type == FLOAT)
            dataframe->columns[col_num].data[i].f =
                (float)atof(lines[i][col_num]);
        if (dataframe->columns[col_num].type == STRING) {
            dataframe->columns[col_num].data[i].str = c_alloc(sizeof(char),
                str_len(lines[i][col_num]) + 1, dataframe->alloc);
            str_cpy(lines[i][col_num], dataframe->columns[col_num].data[i].str);
        }
    }
    return DF_SUCC;
}

static dataframe_t *fill_data_final(dataframe_t *dataframe,
    char ***sep_lines, c_alloc_t *buff_alloc)
{
    for (int i = 0; i < dataframe->nb_columns; i++) {
        if (get_type(dataframe, i, sep_lines) == DF_FAIL ||
            write_line(sep_lines, i, dataframe) == DF_FAIL)
            return free_return(false, dataframe, buff_alloc);
    }
    return free_return(true, dataframe, buff_alloc);
}

static dataframe_t *write_full_data(char **lines,
    dataframe_t *dataframe, c_alloc_t *buff_alloc)
{
    char ***sep_lines = c_alloc(sizeof(char **),
        dataframe->nb_rows + 1, buff_alloc);

    if (!sep_lines)
        return free_return(false, dataframe, buff_alloc);
    sep_lines[dataframe->nb_rows] = NULL;
    for (int i = 0; i < dataframe->nb_columns; i++) {
        dataframe->columns[i].data = c_alloc(sizeof(data_t),
            dataframe->nb_rows, dataframe->alloc);
        if (!dataframe->columns[i].data)
            return free_return(false, dataframe, buff_alloc);
    }
    for (int i = 0; i < dataframe->nb_rows; i++) {
        sep_lines[i] = str_to_array(lines[i + 1],
            (const char *[]){dataframe->sep, NULL}, false, buff_alloc);
        if (!sep_lines[i] || validate_line(dataframe, sep_lines[i]) == DF_FAIL)
            return free_return(false, dataframe, buff_alloc);
    }
    return fill_data_final(dataframe, sep_lines, buff_alloc);
}

static int create_separator(dataframe_t *dataframe, const char *sep)
{
    if (!sep) {
        dataframe->sep = c_alloc(sizeof(char),
            2, dataframe->alloc);
        if (!dataframe->sep)
            return DF_FAIL;
        str_cpy(",", dataframe->sep);
    } else {
        dataframe->sep = c_alloc(sizeof(char),
            str_len(sep) + 1, dataframe->alloc);
        if (!dataframe->sep)
            return DF_FAIL;
        str_cpy(sep, dataframe->sep);
    }
    return DF_SUCC;
}

static dataframe_t *create_dataframe(dataframe_t *dataframe,
    const char *sep, int buff_size, const char buff[buff_size])
{
    c_alloc_t *buff_alloc = c_ini((uint16_t)1000);
    char **lines = NULL;

    if (!buff_alloc)
        return (free_return(false, dataframe, NULL));
    lines = str_to_array(buff,
        (const char *[]){"\n", NULL}, false, buff_alloc);
    if (!lines || !lines[0] || !lines[0][0])
        return free_return(false, dataframe, buff_alloc);
    if (create_separator(dataframe, sep) == DF_FAIL)
        return free_return(false, dataframe, buff_alloc);
    if (handle_first_line(dataframe, lines[0], buff_alloc) == DF_FAIL)
        return free_return(false, dataframe, buff_alloc);
    if (!lines[1] || !lines[1][0])
        return free_return(true, dataframe, buff_alloc);
    for (; lines[dataframe->nb_rows + 1] &&
        lines[dataframe->nb_rows + 1][0]; dataframe->nb_rows++);
    return write_full_data(lines, dataframe, buff_alloc);
}

static char *load_file(const char *filepath, int *size, c_alloc_t *alloc)
{
    int fd = open(filepath, O_RDONLY);
    char *buff = NULL;
    struct stat st;

    if (fd == -1)
        return NULL;
    if (fstat(fd, &st) == -1)
        return NULL;
    buff = c_alloc(sizeof(char), st.st_size + 1, alloc);
    if (!buff)
        return NULL;
    if (read(fd, buff, st.st_size) == -1)
        return NULL;
    buff[st.st_size] = '\0';
    close(fd);
    *size = (int)st.st_size;
    return buff;
}

dataframe_t *df_read_csv(const char *filename, const char *separator)
{
    c_alloc_t *alloc = c_ini((uint16_t)50);
    int size = 0;
    char *buff = NULL;
    dataframe_t *dataframe = NULL;

    if (!alloc)
        return NULL;
    buff = load_file(filename, &size, alloc);
    if (!buff)
        return free_return(false, NULL, alloc);
    dataframe = c_alloc(sizeof(dataframe_t), 1, alloc);
    if (!dataframe)
        return free_return(false, NULL, alloc);
    dataframe->alloc = alloc;
    return create_dataframe(dataframe, separator, size, buff);
}
