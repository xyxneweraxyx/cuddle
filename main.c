/*
** EPITECH PROJECT, 2025
** cuddle - tests pratiques edge cases df_read_csv
** Compile: gcc test_df_read_csv_practical.c -L. -lcuddle -o test_practical
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/dataframe.h"

static int g_passed = 0;
static int g_failed = 0;
static int g_total = 0;

/* ─── helpers ─────────────────────────────────────────────────── */

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

static void check_col_name(const char *name, dataframe_t *df,
    int col_idx, const char *expected_name)
{
    char buf[256];

    if (!df) { fail(name, "df est NULL"); return; }
    if (strcmp(df->columns[col_idx].name, expected_name) == 0) {
        pass(name);
    } else {
        snprintf(buf, sizeof(buf), "col[%d].name = \"%s\", attendu \"%s\"",
            col_idx, df->columns[col_idx].name, expected_name);
        fail(name, buf);
    }
    df_free(df);
}

/* ─── tests newlines ──────────────────────────────────────────── */

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
    check_shape("ligne vide au milieu -> shape (1, 2)", df, 1, 2);
}

/* ─── tests types numeriques limites ─────────────────────────── */

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

/* ─── tests bool ─────────────────────────────────────────────── */

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

/* ─── tests strings ──────────────────────────────────────────── */

static void test_strings(void)
{
    dataframe_t *df;

    section("STRINGS ET CAS SPECIAUX");

    df = df_read_csv("test_csv/spaces_in_header.csv", ",");
    check_col_name("espace dans nom colonne: col[0] = 'first name'",
        df, 0, "first name");

    df = df_read_csv("test_csv/spaces_in_header.csv", ",");
    check_col_name("espace dans nom colonne: col[1] = 'last name'",
        df, 1, "last name");

    df = df_read_csv("test_csv/spaces_in_header.csv", ",");
    check_str_val("valeur apres header avec espace: Smith", df, 1, 0, "Smith");

    df = df_read_csv("test_csv/long_string.csv", ",");
    check_not_null("string longue: non-NULL", df);

    df = df_read_csv("test_csv/long_string.csv", ",");
    check_col_type("string longue: col -> STRING", df, 1, STRING);

    df = df_read_csv("test_csv/long_string.csv", ",");
    check_str_val("string longue: valeur integre",
        df, 1, 0,
        "ceci est une description tres longue qui contient "
        "beaucoup de mots et de caracteres divers");

    df = df_read_csv("test_csv/sep_in_value_semicolon.csv", ";");
    check_shape("sep ';' avec valeurs normales: shape (2, 2)", df, 2, 2);

    df = df_read_csv("test_csv/sep_in_value_semicolon.csv", ";");
    check_str_val("sep ';': col[1][0] = bien", df, 1, 0, "bien");
}

/* ─── tests structure independance ───────────────────────────── */

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

/* ─── main ────────────────────────────────────────────────────── */

int main(void)
{
    printf("\033[1;33m");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║   TEST SUITE PRATIQUES: df_read_csv          ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\033[0m");

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