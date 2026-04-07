# cuddle — C Dataframe Library
 
A pandas-inspired dataframe library in C. Reads and writes CSVs, supports filtering, sorting, groupby, apply, and type casting. Memory is managed through the bundled `c_alloc` allocator.
 
## Build
 
```sh
make           # Produces libcuddle.a
make main      # Produces the cuddle executable
```
 
## Public API
 
```c
// Core
dataframe_t *df_read_csv(const char *filename, const char *separator);
int          df_write_csv(dataframe_t *df, const char *filename);
void         df_free(dataframe_t *df);
 
// Inspection
dataframe_t      *df_head(dataframe_t *df, int n);
dataframe_t      *df_tail(dataframe_t *df, int n);
dataframe_shape_t df_shape(dataframe_t *df);
void              df_info(dataframe_t *df);
void              df_describe(dataframe_t *df);
 
// Transformation
dataframe_t *df_filter(dataframe_t *df, const char *col, bool (*fn)(void *));
dataframe_t *df_sort(dataframe_t *df, const char *col, bool (*fn)(void *, void *));
dataframe_t *df_groupby(dataframe_t *df, const char *by, const char **cols, void *(*agg)(void **, int));
dataframe_t *df_apply(dataframe_t *df, const char *col, void *(*fn)(void *));
dataframe_t *df_to_type(dataframe_t *df, const char *col, column_type_t type);
 
// Utility
void **df_get_values(dataframe_t *df, const char *col);
void  *df_get_value(dataframe_t *df, int row, const char *col);
void **df_get_unique_values(dataframe_t *df, const char *col);
```
 
Supported column types: `BOOL`, `INT`, `UINT`, `FLOAT`, `STRING`.
 
## Author
 
EPITECH PROJECT, 2025 — cuddle