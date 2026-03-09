#include "./include/dataframe.h"

int main(void)
{
    dataframe_t *dataframe = df_read_csv("./test_csv/basic.csv", NULL);
    dataframe_t *new_df = df_tail(dataframe, 2);
    df_write_csv(new_df, "./my_csv.csv");
    df_info(dataframe);
    df_describe(dataframe);
    df_describe(new_df);
    df_free(new_df);
    df_free(dataframe);
    return 0;
}