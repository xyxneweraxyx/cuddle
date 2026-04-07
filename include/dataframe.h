/*
** EPITECH PROJECT, 2025
** dataframe
** File description:
** Header file.
*/

#ifndef DATAFRAME
    #define DATAFRAME

    // Includes
    #include <stdio.h>
    #include <math.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include "./../lib/c_alloc/c_alloc.h"
    #include "./../lib/str/str.h"

    // Exit codes
    #define DF_SUCC 0
    #define DF_FAIL 84

    // Strings
    #define DF_DESCRIBE1 "Column: %s\nCount: %d\nMean: %.2f\nStd"
    #define DF_DESCRIBE2 ": %.2f\nMin: %.2f\nMax: %.2f\n"
    #define DF_DESCRIBE DF_DESCRIBE1 DF_DESCRIBE2

// Typedefs

typedef enum column_type_e {
    BOOL,
    INT,
    UINT,
    FLOAT,
    STRING,
    UNDEFINED
} column_type_t;

typedef union data_u {
    char *str;
    int i;
    unsigned int ui;
    float f;
    bool b;
} data_t;

typedef struct dataframe_shape_s {
    int nb_rows;
    int nb_columns;
} dataframe_shape_t;

typedef struct column_data_s {
    int count;
    float mean;
    float std;
    float min;
    float max;
} column_data_t;

typedef struct column_s {
    char *name;
    data_t *data;
    column_type_t type;
} column_t;

typedef struct dataframe_s {
    int nb_rows;
    int nb_columns;
    char *sep;
    column_t *columns;
    c_alloc_t *alloc;
} dataframe_t;

typedef struct parse_state_s {
    const char *str;
    const char **sep;
    bool skip_empty;
    int cur;
} parse_state_t;

typedef struct groupby_s {
    dataframe_t *old_df;
    dataframe_t *new_df;
    int group_col;
    int new_col;
    void *(*agg_func)(void **, int);
} groupby_t;

// Functions

// Public functions

/// Core functions
dataframe_t *df_read_csv(const char *filename, const char *separator);
int df_write_csv(dataframe_t *dataframe, const char *filename);
void df_free(dataframe_t *dataframe);

/// Basic functions
dataframe_t *df_head(dataframe_t *dataframe, int nb_rows);
dataframe_t *df_tail(dataframe_t *dataframe, int nb_rows);
dataframe_shape_t df_shape(dataframe_t *dataframe);
void df_info(dataframe_t *dataframe);
void df_describe(dataframe_t *dataframe);

/// Transformation functions
dataframe_t *df_filter(dataframe_t *dataframe,
    const char *column, bool (*filter_func)(void *value));
dataframe_t *df_sort(dataframe_t *dataframe,
    const char *column, bool (*sort_func)(void *value1, void *value2));
dataframe_t *df_groupby(dataframe_t *dataframe, const char *aggregate_by, const
    char **to_aggregate, void *(*agg_func)(void **values, int nb_values));
dataframe_t *df_apply(dataframe_t *dataframe,
    const char *column, void *(*apply_func)(void *value));
dataframe_t *df_to_type(dataframe_t *dataframe,
    const char *column, column_type_t downcast);

/// Utility functions
void **df_get_values(dataframe_t *dataframe, const char *column);
void *df_get_value(dataframe_t *dataframe, int row, const char *column);
void **df_get_unique_values(dataframe_t *dataframe, const char *column);

// Private functions
dataframe_t *new_dataframe(dataframe_t *old_df);
dataframe_t *free_return(bool success,
    dataframe_t *dataframe, c_alloc_t *alloc_to_free);
char **str_to_array(const char *str, const char **sep,
    bool skip_empty, c_alloc_t *alloc);
int copy_all_data(dataframe_t *old_df, dataframe_t *new_df);
int validate_line(dataframe_t *dataframe, char **values);

// Groupby functions
int find_col(dataframe_t *df, const char *name);
bool data_eq(data_t a, data_t b, column_type_t type);
bool is_first_occ(dataframe_t *df, int col, int row);
int count_in_group(groupby_t *gb, int ref_row);
void **collect_group(groupby_t *gb, int ref_row, int agg_col);
int match_sep(const char *str, const char **seps);
void skip_seps(parse_state_t *ps);
void skip_one_word(parse_state_t *ps);

#endif
