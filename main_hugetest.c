/*
** EPITECH PROJECT, 2025
** cuddle - test suite complète df_read_csv
** Compile: gcc test_df_read_csv_all.c -L. -lcuddle -o test_all
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/dataframe.h"

static int g_passed = 0;
static int g_failed = 0;
static int g_total = 0;

static void pass(const char *name)
{
    printf("  \033[32m[PASS]\033[0m %s\n", name);
    g_passed++;
    g_total++;
}

static void fail(const char *name, const char *reason)
{
    printf("  \033[31m[FAIL]\033[0m %s\n         → %s\n", name, reason);
    g_failed++;
    g_total++;
}

static void section(const char *title)
{
    printf("\n\033[1;34m══ %s ══\033[0m\n", title);
}

static const char *type_str(column_type_t t)
{
    if (t == BOOL)      return "BOOL";
    if (t == INT)       return "INT";
    if (t == UINT)      return "UINT";
    if (t == FLOAT)     return "FLOAT";
    if (t == STRING)    return "STRING";
    if (t == UNDEFINED) return "UNDEFINED";
    return "???";
}

static void check_not_null(const char *name, dataframe_t *df)
{
    if (df) { pass(name); df_free(df); }
    else    fail(name, "retourne NULL, attendu non-NULL");
}

static void check_null(const char *name, dataframe_t *df)
{
    if (!df) { pass(name); return; }
    fail(name, "retourne non-NULL, attendu NULL");
    df_free(df);
}

static void check_shape(const char *name, dataframe_t *df,
    int exp_rows, int exp_cols)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    if (df->nb_rows == exp_rows && df->nb_columns == exp_cols) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "shape (%d, %d) attendu (%d, %d)",
            df->nb_rows, df->nb_columns, exp_rows, exp_cols);
        fail(name, buf);
    }
    df_free(df);
}

static void check_col_type(const char *name, dataframe_t *df,
    int col_idx, column_type_t expected)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    if (col_idx >= df->nb_columns) {
        snprintf(buf, sizeof(buf), "col_idx %d >= nb_columns %d",
            col_idx, df->nb_columns);
        fail(name, buf);
        df_free(df);
        return;
    }
    if (df->columns[col_idx].type == expected) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d] = %s, attendu %s",
            col_idx, type_str(df->columns[col_idx].type), type_str(expected));
        fail(name, buf);
    }
    df_free(df);
}

static void check_col_name(const char *name, dataframe_t *df,
    int col_idx, const char *expected_name)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    if (col_idx >= df->nb_columns) {
        snprintf(buf, sizeof(buf), "col_idx %d >= nb_columns %d",
            col_idx, df->nb_columns);
        fail(name, buf);
        df_free(df);
        return;
    }
    if (strcmp(df->columns[col_idx].name, expected_name) == 0) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d].name = \"%s\", attendu \"%s\"",
            col_idx, df->columns[col_idx].name, expected_name);
        fail(name, buf);
    }
    df_free(df);
}

static void check_str_val(const char *name, dataframe_t *df,
    int col, int row, const char *expected)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    char *got = df->columns[col].data[row].str;
    if (got && strcmp(got, expected) == 0) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d][%d] = \"%s\", attendu \"%s\"",
            col, row, got ? got : "NULL", expected);
        fail(name, buf);
    }
    df_free(df);
}

static void check_int_val(const char *name, dataframe_t *df,
    int col, int row, int expected)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    int got = df->columns[col].data[row].i;
    if (got == expected) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d][%d].i = %d, attendu %d",
            col, row, got, expected);
        fail(name, buf);
    }
    df_free(df);
}

static void check_uint_val(const char *name, dataframe_t *df,
    int col, int row, unsigned int expected)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    unsigned int got = df->columns[col].data[row].ui;
    if (got == expected) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d][%d].ui = %u, attendu %u",
            col, row, got, expected);
        fail(name, buf);
    }
    df_free(df);
}

static void check_float_val(const char *name, dataframe_t *df,
    int col, int row, float expected, float eps)
{
    char buf[256];
    float got;
    float diff;

    if (!df) { fail(name, "df est NULL"); return; }
    got = df->columns[col].data[row].f;
    diff = got - expected;
    if (diff < 0) diff = -diff;
    if (diff < eps) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d][%d].f = %f, attendu %f",
            col, row, got, expected);
        fail(name, buf);
    }
    df_free(df);
}

static void check_bool_val(const char *name, dataframe_t *df,
    int col, int row, int expected)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    int got = (int)df->columns[col].data[row].b;
    if (got == expected) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d][%d].b = %d, attendu %d",
            col, row, got, expected);
        fail(name, buf);
    }
    df_free(df);
}

static void test_basics(void)
{
    dataframe_t *df;

    section("BASIQUES");

    df = df_read_csv("test_csv/basic.csv", ",");
    check_shape("basic: shape (3, 3)", df, 3, 3);

    df = df_read_csv("test_csv/basic.csv", ",");
    check_col_name("basic: col[0] = name", df, 0, "name");

    df = df_read_csv("test_csv/basic.csv", ",");
    check_col_name("basic: col[1] = age", df, 1, "age");

    df = df_read_csv("test_csv/basic.csv", ",");
    check_col_name("basic: col[2] = city", df, 2, "city");

    df = df_read_csv("test_csv/semicolon.csv", ";");
    check_shape("separateur ';': shape (2, 3)", df, 2, 3);

    df = df_read_csv("test_csv/tab.csv", "\t");
    check_shape("separateur tab: shape (2, 3)", df, 2, 3);

    df = df_read_csv("test_csv/sep_multichar.csv", "<->");
    check_shape("separateur multi-char '<->': shape (2, 3)", df, 2, 3);

    df = df_read_csv("test_csv/basic.csv", NULL);
    check_not_null("separator NULL -> par defaut ','", df);

    df = df_read_csv("test_csv/header_only.csv", ",");
    check_shape("header only: 0 lignes, 3 colonnes", df, 0, 3);

    df = df_read_csv("test_csv/single_row.csv", ",");
    check_shape("single row: shape (1, 2)", df, 1, 2);

    df = df_read_csv("test_csv/single_column.csv", ",");
    check_shape("single column: shape (3, 1)", df, 3, 1);

    df = df_read_csv("test_csv/one_column_one_row.csv", ",");
    check_shape("1x1: shape (1, 1)", df, 1, 1);

    df = df_read_csv("test_csv/many_columns.csv", ",");
    check_shape("many columns: shape (1, 10)", df, 1, 10);
}

static void test_types(void)
{
    dataframe_t *df;

    section("DETECTION DE TYPES");

    df = df_read_csv("test_csv/types_uint.csv", ",");
    check_col_type("uint: col score -> UINT", df, 1, UINT);

    df = df_read_csv("test_csv/types_int.csv", ",");
    check_col_type("int: col temperature -> INT", df, 1, INT);

    df = df_read_csv("test_csv/types_float.csv", ",");
    check_col_type("float: col price -> FLOAT", df, 1, FLOAT);

    df = df_read_csv("test_csv/types_bool.csv", ",");
    check_col_type("bool: col active -> BOOL", df, 1, BOOL);

    df = df_read_csv("test_csv/types_string.csv", ",");
    check_col_type("string: col label -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/types_mixed_int_uint.csv", ",");
    check_col_type("mixed int/uint: -> INT (negatif present)", df, 1, INT);

    df = df_read_csv("test_csv/types_mixed_float_int.csv", ",");
    check_col_type("mixed float/int: -> FLOAT", df, 1, FLOAT);

    df = df_read_csv("test_csv/types_mixed_string.csv", ",");
    check_col_type("mixed string: -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/types_bool_case_insensitive.csv", ",");
    check_col_type("bool insensible a la casse: -> BOOL", df, 1, BOOL);

    df = df_read_csv("test_csv/invalid_float.csv", ",");
    check_col_type("25.0.0 -> STRING (double point)", df, 1, STRING);

    df = df_read_csv("test_csv/space_in_number.csv", ",");
    check_col_type("'10 000' -> STRING (espace)", df, 1, STRING);

    df = df_read_csv("test_csv/bool_mixed_with_string.csv", ",");
    check_col_type("bool + 'yes' -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/negative_only.csv", ",");
    check_col_type("negatifs seulement -> INT", df, 1, INT);

    df = df_read_csv("test_csv/float_no_decimal.csv", ",");
    check_col_type("1.0 2.0 3.0 -> FLOAT", df, 1, FLOAT);

    df = df_read_csv("test_csv/negative_zero.csv", ",");
    check_col_type("-0 -> INT", df, 1, INT);

    df = df_read_csv("test_csv/just_dot.csv", ",");
    check_col_type("'.' seul -> STRING", df, 1, STRING);
}

static void test_values(void)
{
    dataframe_t *df;

    section("VALEURS STOCKEES");

    df = df_read_csv("test_csv/basic.csv", ",");
    check_str_val("basic: col[0][0] = 'Alice'", df, 0, 0, "Alice");

    df = df_read_csv("test_csv/basic.csv", ",");
    check_str_val("basic: col[0][2] = 'Charlie'", df, 0, 2, "Charlie");

    df = df_read_csv("test_csv/basic.csv", ",");
    check_uint_val("basic: col[1][0] = 25", df, 1, 0, 25);

    df = df_read_csv("test_csv/basic.csv", ",");
    check_uint_val("basic: col[1][1] = 30", df, 1, 1, 30);

    df = df_read_csv("test_csv/basic.csv", ",");
    check_uint_val("basic: col[1][2] = 35", df, 1, 2, 35);

    df = df_read_csv("test_csv/types_int.csv", ",");
    check_int_val("int: col[1][1] = -3", df, 1, 1, -3);

    df = df_read_csv("test_csv/types_float.csv", ",");
    check_float_val("float: col[1][1] = 3.14", df, 1, 1, 3.14f, 0.01f);

    df = df_read_csv("test_csv/types_float.csv", ",");
    check_float_val("float: col[1][0] = 9.99", df, 1, 0, 9.99f, 0.01f);

    df = df_read_csv("test_csv/types_bool.csv", ",");
    check_bool_val("bool: col[1][0] = true (1)", df, 1, 0, 1);

    df = df_read_csv("test_csv/types_bool.csv", ",");
    check_bool_val("bool: col[1][1] = false (0)", df, 1, 1, 0);

    df = df_read_csv("test_csv/types_bool.csv", ",");
    check_bool_val("bool: col[1][2] = TRUE (1)", df, 1, 2, 1);

    df = df_read_csv("test_csv/many_columns.csv", ",");
    check_uint_val("many_cols: col[9][0] = 10", df, 9, 0, 10);

    df = df_read_csv("test_csv/negative_only.csv", ",");
    check_int_val("negatif: col[1][0] = -10", df, 1, 0, -10);

    df = df_read_csv("test_csv/negative_only.csv", ",");
    check_int_val("negatif: col[1][2] = -5", df, 1, 2, -5);
}

static void test_errors(void)
{
    dataframe_t *df;

    section("ERREURS ATTENDUES (NULL)");

    df = df_read_csv("test_csv/error_too_many_cols.csv", ",");
    check_null("trop de colonnes sur une ligne -> NULL", df);

    df = df_read_csv("test_csv/error_too_few_cols.csv", ",");
    check_null("pas assez de colonnes -> NULL", df);

    df = df_read_csv("test_csv/error_empty_file.csv", ",");
    check_null("fichier vide -> NULL", df);

    df = df_read_csv("fichier_inexistant.csv", ",");
    check_null("fichier inexistant -> NULL", df);

    df = df_read_csv(NULL, ",");
    check_null("filename NULL -> NULL", df);
}

static void test_edge_cases(void)
{
    dataframe_t *df;

    section("EDGE CASES GENERAUX");

    df = df_read_csv("test_csv/sep_null_default.csv", NULL);
    check_shape("sep NULL -> ',' par defaut, shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/sep_null_default.csv", NULL);
    check_uint_val("sep NULL: col[1][0] = 25", df, 1, 0, 25);

    df = df_read_csv("test_csv/sep_multichar.csv", "<->");
    check_str_val("sep multi-char: col[0][0] = Alice", df, 0, 0, "Alice");

    df = df_read_csv("test_csv/sep_multichar.csv", "<->");
    check_uint_val("sep multi-char: col[1][1] = 30", df, 1, 1, 30);

    df = df_read_csv("test_csv/large_numbers.csv", ",");
    check_uint_val("large uint: col[1][0] = 999999999", df, 1, 0, 999999999);

    df = df_read_csv("test_csv/types_string.csv", ",");
    check_str_val("string avec espace: col[1][2] = 'foo bar'", df, 1, 2, "foo bar");

    df = df_read_csv("test_csv/single_column.csv", ",");
    check_str_val("single col: col[0][0] = Alice", df, 0, 0, "Alice");

    df = df_read_csv("test_csv/single_column.csv", ",");
    check_str_val("single col: col[0][2] = Charlie", df, 0, 2, "Charlie");

    df = df_read_csv("test_csv/header_only.csv", ",");
    if (df) {
        if (df->nb_rows == 0 && df->nb_columns == 3)
            pass("header only: 0 rows et 3 colonnes");
        else
            fail("header only: mauvaise shape", "attendu 0 rows, 3 cols");
        df_free(df);
    } else {
        pass("header only: retourne NULL (acceptable)");
    }
}

static void test_single_type_edges(void)
{
    dataframe_t *df;

    section("get_single_type - EDGE CASES");

    df = df_read_csv("test_csv/edge_dash_only.csv", ",");
    check_col_type("'-' seul -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_dot_prefix.csv", ",");
    check_col_type("'.5' -> FLOAT", df, 1, FLOAT);

    df = df_read_csv("test_csv/edge_empty_field.csv", ",");
    check_col_type("champ vide -> STRING (pas UINT)", df, 1, STRING);

    df = df_read_csv("test_csv/edge_all_empty.csv", ",");
    check_col_type("tous vides: col[0] -> STRING", df, 0, STRING);

    df = df_read_csv("test_csv/edge_all_empty.csv", ",");
    check_col_type("tous vides: col[1] -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_dash_only_col.csv", ",");
    check_col_type("colonne entiere '-' -> STRING", df, 1, STRING);
}

static void test_type_promotion_bool(void)
{
    dataframe_t *df;

    section("MONTEE DE TYPE - BOOL + NUMERIQUE");

    df = df_read_csv("test_csv/edge_bool_uint.csv", ",");
    check_col_type("BOOL + UINT -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_bool_int.csv", ",");
    check_col_type("BOOL + INT -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_bool_float.csv", ",");
    check_col_type("BOOL + FLOAT -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_uint_bool.csv", ",");
    check_col_type("UINT + BOOL -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_int_bool.csv", ",");
    check_col_type("INT + BOOL -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/edge_float_bool.csv", ",");
    check_col_type("FLOAT + BOOL -> STRING", df, 1, STRING);
}

static void test_newlines(void)
{
    dataframe_t *df;

    section("NEWLINES ET FIN DE FICHIER");

    df = df_read_csv("test_csv/no_trailing_newline.csv", ",");
    check_not_null("sans newline finale: non-NULL", df);

    df = df_read_csv("test_csv/no_trailing_newline.csv", ",");
    check_shape("sans newline finale: shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/no_trailing_newline.csv", ",");
    check_str_val("sans newline finale: col[0][0] = Alice", df, 0, 0, "Alice");

    df = df_read_csv("test_csv/no_trailing_newline.csv", ",");
    check_str_val("sans newline finale: col[0][1] = Bob", df, 0, 1, "Bob");

    df = df_read_csv("test_csv/no_trailing_newline.csv", ",");
    check_uint_val("sans newline finale: col[1][1] = 30", df, 1, 1, 30);

    df = df_read_csv("test_csv/one_trailing_newline.csv", ",");
    check_shape("avec newline finale: shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/multi_trailing_newline.csv", ",");
    check_shape("plusieurs newlines finales: shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/header_only_no_newline.csv", ",");
    if (df) {
        if (df->nb_rows == 0 && df->nb_columns == 3)
            pass("header sans newline: 0 rows 3 cols");
        else
            fail("header sans newline: mauvaise shape", "attendu (0, 3)");
        df_free(df);
    } else {
        pass("header sans newline: retourne NULL (acceptable)");
    }

    df = df_read_csv("test_csv/empty_line_middle.csv", ",");
    check_shape("ligne vide au milieu: lecture partielle (1, 2)", df, 1, 2);
}

static void test_numeric_limits(void)
{
    dataframe_t *df;

    section("VALEURS NUMERIQUES LIMITES");

    df = df_read_csv("test_csv/zero_values.csv", ",");
    check_col_type("colonne de zeros -> UINT", df, 1, UINT);

    df = df_read_csv("test_csv/zero_values.csv", ",");
    check_uint_val("zero: col[1][0] = 0", df, 1, 0, 0);

    df = df_read_csv("test_csv/mixed_zero_neg.csv", ",");
    check_col_type("0 puis -1 -> INT", df, 1, INT);

    df = df_read_csv("test_csv/mixed_zero_neg.csv", ",");
    check_int_val("0 puis -1: col[1][0] = 0", df, 1, 0, 0);

    df = df_read_csv("test_csv/mixed_zero_neg.csv", ",");
    check_int_val("0 puis -1: col[1][1] = -1", df, 1, 1, -1);

    df = df_read_csv("test_csv/float_zero.csv", ",");
    check_col_type("0.0 1.0 -> FLOAT", df, 1, FLOAT);

    df = df_read_csv("test_csv/float_zero.csv", ",");
    check_float_val("0.0 -> 0.0f", df, 1, 0, 0.0f, 0.001f);

    df = df_read_csv("test_csv/float_then_int.csv", ",");
    check_col_type("float puis int -> FLOAT", df, 1, FLOAT);

    df = df_read_csv("test_csv/float_then_int.csv", ",");
    check_float_val("float puis int: col[1][1] = 10.0", df, 1, 1, 10.0f, 0.01f);

    df = df_read_csv("test_csv/minus_zero.csv", ",");
    check_col_type("-0 -> INT", df, 1, INT);

    df = df_read_csv("test_csv/minus_zero.csv", ",");
    check_int_val("-0: col[1][0] = 0", df, 1, 0, 0);

    df = df_read_csv("test_csv/uint_max_ish.csv", ",");
    check_col_type("2147483647 -> UINT", df, 1, UINT);

    df = df_read_csv("test_csv/uint_max_ish.csv", ",");
    check_uint_val("2147483647: valeur correcte", df, 1, 0, 2147483647);
}

static void test_bool_values(void)
{
    dataframe_t *df;

    section("VALEURS BOOL - TOUTES LES CASSES");

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_col_type("true/True/TRUE/false/False/FALSE -> BOOL", df, 1, BOOL);

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_bool_val("true -> 1", df, 1, 0, 1);

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_bool_val("True -> 1", df, 1, 1, 1);

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_bool_val("TRUE -> 1", df, 1, 2, 1);

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_bool_val("false -> 0", df, 1, 3, 0);

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_bool_val("False -> 0", df, 1, 4, 0);

    df = df_read_csv("test_csv/bool_exact_cases.csv", ",");
    check_bool_val("FALSE -> 0", df, 1, 5, 0);
}

static void test_strings(void)
{
    dataframe_t *df;

    section("STRINGS ET CAS SPECIAUX");

    df = df_read_csv("test_csv/spaces_in_header.csv", ",");
    check_col_name("espace dans header: col[0] = 'first name'",
        df, 0, "first name");

    df = df_read_csv("test_csv/spaces_in_header.csv", ",");
    check_col_name("espace dans header: col[1] = 'last name'",
        df, 1, "last name");

    df = df_read_csv("test_csv/spaces_in_header.csv", ",");
    check_str_val("valeur apres header avec espace: Smith", df, 1, 0, "Smith");

    df = df_read_csv("test_csv/long_string.csv", ",");
    check_not_null("string longue: non-NULL", df);

    df = df_read_csv("test_csv/long_string.csv", ",");
    check_col_type("string longue: col -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/long_string.csv", ",");
    check_str_val("string longue: valeur integre", df, 1, 0,
        "ceci est une description tres longue qui contient "
        "beaucoup de mots et de caracteres divers");

    df = df_read_csv("test_csv/sep_in_value_semicolon.csv", ";");
    check_shape("sep ';' avec valeurs normales: shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/sep_in_value_semicolon.csv", ";");
    check_str_val("sep ';': col[1][0] = bien", df, 1, 0, "bien");
}

static void test_structure(void)
{
    dataframe_t *df;

    section("STRUCTURE DU DATAFRAME");

    df = df_read_csv("test_csv/one_sep.csv", ",");
    check_shape("2 colonnes 1 row: shape (1, 2)", df, 1, 2);

    df = df_read_csv("test_csv/one_sep.csv", ",");
    check_uint_val("2 colonnes: col[0][0] = 1", df, 0, 0, 1);

    df = df_read_csv("test_csv/one_sep.csv", ",");
    check_uint_val("2 colonnes: col[1][0] = 2", df, 1, 0, 2);

    df = df_read_csv("test_csv/single_value.csv", ",");
    check_shape("single value: shape (1, 1)", df, 1, 1);

    df = df_read_csv("test_csv/single_value.csv", ",");
    check_uint_val("single value: col[0][0] = 42", df, 0, 0, 42);

    df = df_read_csv("test_csv/all_zeros.csv", ",");
    check_shape("all zeros: shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/all_zeros.csv", ",");
    check_uint_val("all zeros: col[1][0] = 0", df, 1, 0, 0);

    df = df_read_csv("test_csv/all_zeros.csv", ",");
    check_uint_val("all zeros: col[1][1] = 0", df, 1, 1, 0);

    df = df_read_csv("test_csv/many_columns.csv", ",");
    check_uint_val("10 colonnes: col[0][0] = 1", df, 0, 0, 1);

    df = df_read_csv("test_csv/many_columns.csv", ",");
    check_uint_val("10 colonnes: col[4][0] = 5", df, 4, 0, 5);

    df = df_read_csv("test_csv/many_columns.csv", ",");
    check_uint_val("10 colonnes: col[9][0] = 10", df, 9, 0, 10);
}

int main(void)
{
    printf("\033[1;33m");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║     TEST SUITE COMPLETE: df_read_csv         ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\033[0m");

    test_basics();
    test_types();
    test_values();
    test_errors();
    test_edge_cases();
    test_single_type_edges();
    test_type_promotion_bool();
    test_newlines();
    test_numeric_limits();
    test_bool_values();
    test_strings();
    test_structure();

    printf("\n\033[1m══════════════════════════════════════════════\033[0m\n");
    printf("Total  : %d tests\n", g_total);
    printf("\033[32mPassés : %d\033[0m\n", g_passed);
    printf("\033[31mÉchoués: %d\033[0m\n", g_failed);
    printf("\033[1m══════════════════════════════════════════════\033[0m\n");

    return (g_failed > 0) ? 84 : 0;
}
*/