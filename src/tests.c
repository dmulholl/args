// -----------------------------------------------------------------------------
// Unit test suite.
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "args.h"

// -----------------------------------------------------------------------------
// 1. Flags.
// -----------------------------------------------------------------------------

void test_flag_empty() {
    ArgParser *parser = ap_new();
    ap_flag(parser, "foo f");
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    ap_free(parser);
    printf(".");
}

void test_flag_missing() {
    ArgParser *parser = ap_new();
    ap_flag(parser, "foo f");
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    ap_free(parser);
    printf(".");
}

void test_flag_long() {
    ArgParser *parser = ap_new();
    ap_flag(parser, "foo f");
    ap_parse(parser, 2, (char *[]){"", "--foo"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    ap_free(parser);
    printf(".");
}

void test_flag_short() {
    ArgParser *parser = ap_new();
    ap_flag(parser, "foo f");
    ap_parse(parser, 2, (char *[]){"", "-f"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    ap_free(parser);
    printf(".");
}

void test_flag_condensed() {
    ArgParser *parser = ap_new();
    ap_flag(parser, "foo f");
    ap_parse(parser, 2, (char *[]){"", "-fff"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 3);
    ap_free(parser);
    printf(".");
}

void test_flag_multi() {
    ArgParser *parser = ap_new();
    ap_flag(parser, "foo f");
    ap_parse(parser, 4, (char *[]){"", "-fff", "--foo", "-f"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 5);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 2. String-valued options.
// -----------------------------------------------------------------------------

void test_str_opt_default() {
    ArgParser *parser = ap_new();
    ap_str_opt(parser, "foo f", "default");
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    assert(strcmp(ap_str_value(parser, "foo"), "default") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_long() {
    ArgParser *parser = ap_new();
    ap_str_opt(parser, "foo f", "default");
    ap_parse(parser, 3, (char *[]){"", "--foo", "bar"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(strcmp(ap_str_value(parser, "foo"), "bar") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_short() {
    ArgParser *parser = ap_new();
    ap_str_opt(parser, "foo f", "default");
    ap_parse(parser, 3, (char *[]){"", "-f", "bar"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(strcmp(ap_str_value(parser, "foo"), "bar") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_condensed() {
    ArgParser *parser = ap_new();
    ap_str_opt(parser, "foo f", "default");
    ap_parse(parser, 4, (char *[]){"", "-ff", "bar", "baz"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 2);
    assert(strcmp(ap_str_value(parser, "foo"), "baz") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_multi() {
    ArgParser *parser = ap_new();
    ap_str_opt(parser, "foo f", "default");
    ap_parse(parser, 6, (char *[]){"", "-ff", "bar", "baz", "--foo", "bam"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 3);
    assert(strcmp(ap_str_value(parser, "foo"), "bam") == 0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 3. Integer-valued options.
// -----------------------------------------------------------------------------

void test_int_opt_default() {
    ArgParser *parser = ap_new();
    ap_int_opt(parser, "foo f", 123);
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    assert(ap_int_value(parser, "foo") == 123);
    ap_free(parser);
    printf(".");
}

void test_int_opt_long() {
    ArgParser *parser = ap_new();
    ap_int_opt(parser, "foo f", 123);
    ap_parse(parser, 3, (char *[]){"", "--foo", "456"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_int_value(parser, "foo") == 456);
    ap_free(parser);
    printf(".");
}

void test_int_opt_short() {
    ArgParser *parser = ap_new();
    ap_int_opt(parser, "foo f", 123);
    ap_parse(parser, 3, (char *[]){"", "-f", "456"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_int_value(parser, "foo") == 456);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 4. Double-valued options.
// -----------------------------------------------------------------------------

void test_dbl_opt_default() {
    ArgParser *parser = ap_new();
    ap_dbl_opt(parser, "foo f", 123.0);
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    assert(ap_dbl_value(parser, "foo") == 123.0);
    ap_free(parser);
    printf(".");
}

void test_dbl_opt_long() {
    ArgParser *parser = ap_new();
    ap_dbl_opt(parser, "foo f", 123.0);
    ap_parse(parser, 3, (char *[]){"", "--foo", "456.0"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_dbl_value(parser, "foo") == 456.0);
    ap_free(parser);
    printf(".");
}

void test_dbl_opt_short() {
    ArgParser *parser = ap_new();
    ap_dbl_opt(parser, "foo f", 123.0);
    ap_parse(parser, 3, (char *[]){"", "-f", "456.0"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_dbl_value(parser, "foo") == 456.0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 5. Positional arguments.
// -----------------------------------------------------------------------------

void test_pos_args() {
    ArgParser *parser = ap_new();
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_has_args(parser) == true);
    assert(ap_count_args(parser) == 2);
    assert(strcmp(ap_arg(parser, 0), "abc") == 0);
    assert(strcmp(ap_arg(parser, 1), "def") == 0);
    ap_free(parser);
    printf(".");
}

void test_pos_args_as_ints() {
    ArgParser *parser = ap_new();
    ap_parse(parser, 3, (char *[]){"", "123", "456"});
    assert(ap_args_as_ints(parser)[0] == 123);
    assert(ap_args_as_ints(parser)[1] == 456);
    ap_free(parser);
    printf(".");
}

void test_pos_args_as_doubles() {
    ArgParser *parser = ap_new();
    ap_parse(parser, 3, (char *[]){"", "123", "456"});
    assert(ap_args_as_doubles(parser)[0] == 123.0);
    assert(ap_args_as_doubles(parser)[1] == 456.0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 6. Option parsing switch.
// -----------------------------------------------------------------------------

void test_option_parsing_switch() {
    ArgParser *parser = ap_new();
    ap_parse(parser, 5, (char *[]){"", "foo", "--", "--bar", "--baz"});
    assert(ap_count_args(parser) == 3);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 7. Commands.
// -----------------------------------------------------------------------------

void test_command() {
    ArgParser *parser = ap_new();
    ArgParser *cmd_parser = ap_cmd(parser, "cmd");
    ap_flag(cmd_parser, "foo");
    ap_int_opt(cmd_parser, "bar", 123);
    ap_parse(parser, 7, (char *[]){
        "",
        "cmd",
        "abc", "def",
        "--foo",
        "--bar", "456",
    });
    assert(ap_has_cmd(parser) == true);
    assert(strcmp(ap_cmd_name(parser), "cmd") == 0);
    assert(ap_cmd_parser(parser) == cmd_parser);
    assert(ap_has_args(cmd_parser) == true);
    assert(ap_count_args(cmd_parser) == 2);
    assert(ap_found(cmd_parser, "foo") == true);
    assert(ap_int_value(cmd_parser, "bar") == 456);
    printf(".");
}

// -----------------------------------------------------------------------------
// Test runner.
// -----------------------------------------------------------------------------

void line() {
    for (int i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n");
}

int main() {
    setbuf(stdout, NULL);
    line();

    printf("Tests: 1 ");
    test_flag_empty();
    test_flag_missing();
    test_flag_long();
    test_flag_short();
    test_flag_condensed();
    test_flag_multi();

    printf(" 2 ");
    test_str_opt_default();
    test_str_opt_long();
    test_str_opt_short();
    test_str_opt_condensed();
    test_str_opt_multi();

    printf(" 3 ");
    test_int_opt_default();
    test_int_opt_long();
    test_int_opt_short();

    printf(" 4 ");
    test_dbl_opt_default();
    test_dbl_opt_long();
    test_dbl_opt_short();

    printf(" 5 ");
    test_pos_args();
    test_pos_args_as_ints();
    test_pos_args_as_doubles();

    printf(" 6 ");
    test_option_parsing_switch();

    printf(" 7 ");
    test_command();

    printf(" [ok]\n");
    line();
}
