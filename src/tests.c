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

void test_flag_empty(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "foo f");
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    ap_free(parser);
    printf(".");
}

void test_flag_missing(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "foo f");
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    ap_free(parser);
    printf(".");
}

void test_flag_long(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "foo f");
    ap_parse(parser, 2, (char *[]){"", "--foo"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    ap_free(parser);
    printf(".");
}

void test_flag_short(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "foo f");
    ap_parse(parser, 2, (char *[]){"", "-f"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    ap_free(parser);
    printf(".");
}

void test_flag_condensed(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "foo f");
    ap_parse(parser, 2, (char *[]){"", "-fff"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 3);
    ap_free(parser);
    printf(".");
}

void test_flag_multi(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "foo f");
    ap_parse(parser, 4, (char *[]){"", "-fff", "--foo", "-f"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 5);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 2. String-valued options.
// -----------------------------------------------------------------------------

void test_str_opt_default(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_str_opt(parser, "foo f", "default");
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    assert(strcmp(ap_get_str_value(parser, "foo"), "default") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_long(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_str_opt(parser, "foo f", "default");
    ap_parse(parser, 3, (char *[]){"", "--foo", "bar"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(strcmp(ap_get_str_value(parser, "foo"), "bar") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_short(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_str_opt(parser, "foo f", "default");
    ap_parse(parser, 3, (char *[]){"", "-f", "bar"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(strcmp(ap_get_str_value(parser, "foo"), "bar") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_condensed(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_str_opt(parser, "foo f", "default");
    ap_parse(parser, 4, (char *[]){"", "-ff", "bar", "baz"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 2);
    assert(strcmp(ap_get_str_value(parser, "foo"), "baz") == 0);
    ap_free(parser);
    printf(".");
}

void test_str_opt_multi(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_str_opt(parser, "foo f", "default");
    ap_parse(parser, 6, (char *[]){"", "-ff", "bar", "baz", "--foo", "bam"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 3);
    assert(strcmp(ap_get_str_value(parser, "foo"), "bam") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "foo", 0), "bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "foo", 1), "baz") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "foo", 2), "bam") == 0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 3. Integer-valued options.
// -----------------------------------------------------------------------------

void test_int_opt_default(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_int_opt(parser, "foo f", 123);
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    assert(ap_get_int_value(parser, "foo") == 123);
    ap_free(parser);
    printf(".");
}

void test_int_opt_long(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_int_opt(parser, "foo f", 123);
    ap_parse(parser, 3, (char *[]){"", "--foo", "456"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_get_int_value(parser, "foo") == 456);
    ap_free(parser);
    printf(".");
}

void test_int_opt_short(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_int_opt(parser, "foo f", 123);
    ap_parse(parser, 3, (char *[]){"", "-f", "456"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_get_int_value(parser, "foo") == 456);
    ap_free(parser);
    printf(".");
}

void test_int_opt_multi(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_int_opt(parser, "foo f", 999);
    ap_parse(parser, 6, (char *[]){"", "-ff", "123", "456", "--foo", "789"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 3);
    assert(ap_get_int_value(parser, "foo") == 789);
    assert(ap_get_int_value_at_index(parser, "foo", 0) == 123);
    assert(ap_get_int_value_at_index(parser, "foo", 1) == 456);
    assert(ap_get_int_value_at_index(parser, "foo", 2) == 789);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 4. Double-valued options.
// -----------------------------------------------------------------------------

void test_dbl_opt_default(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_dbl_opt(parser, "foo f", 123.0);
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_found(parser, "foo") == false);
    assert(ap_count(parser, "foo") == 0);
    assert(ap_get_dbl_value(parser, "foo") == 123.0);
    ap_free(parser);
    printf(".");
}

void test_dbl_opt_long(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_dbl_opt(parser, "foo f", 123.0);
    ap_parse(parser, 3, (char *[]){"", "--foo", "456.0"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_get_dbl_value(parser, "foo") == 456.0);
    ap_free(parser);
    printf(".");
}

void test_dbl_opt_short(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_dbl_opt(parser, "foo f", 123.0);
    ap_parse(parser, 3, (char *[]){"", "-f", "456.0"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 1);
    assert(ap_get_dbl_value(parser, "foo") == 456.0);
    ap_free(parser);
    printf(".");
}

void test_dbl_opt_multi(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_dbl_opt(parser, "foo f", 999.0);
    ap_parse(parser, 6, (char *[]){"", "-ff", "123.0", "456.0", "--foo", "789.0"});
    assert(ap_found(parser, "foo") == true);
    assert(ap_count(parser, "foo") == 3);
    assert(ap_get_dbl_value(parser, "foo") == 789.0);
    assert(ap_get_dbl_value_at_index(parser, "foo", 0) == 123.0);
    assert(ap_get_dbl_value_at_index(parser, "foo", 1) == 456.0);
    assert(ap_get_dbl_value_at_index(parser, "foo", 2) == 789.0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 5. Positional arguments.
// -----------------------------------------------------------------------------

void test_pos_args(void) {
    ArgParser *parser = ap_new_parser();
    ap_parse(parser, 3, (char *[]){"", "abc", "def"});
    assert(ap_has_args(parser) == true);
    assert(ap_count_args(parser) == 2);
    assert(strcmp(ap_get_arg_at_index(parser, 0), "abc") == 0);
    assert(strcmp(ap_get_arg_at_index(parser, 1), "def") == 0);
    ap_free(parser);
    printf(".");
}

void test_pos_args_as_ints(void) {
    ArgParser *parser = ap_new_parser();
    ap_parse(parser, 3, (char *[]){"", "123", "456"});
    assert(ap_get_args_as_ints(parser)[0] == 123);
    assert(ap_get_args_as_ints(parser)[1] == 456);
    ap_free(parser);
    printf(".");
}

void test_pos_args_as_doubles(void) {
    ArgParser *parser = ap_new_parser();
    ap_parse(parser, 3, (char *[]){"", "123", "456"});
    assert(ap_get_args_as_doubles(parser)[0] == 123.0);
    assert(ap_get_args_as_doubles(parser)[1] == 456.0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 6. Option parsing switch.
// -----------------------------------------------------------------------------

void test_option_parsing_switch(void) {
    ArgParser *parser = ap_new_parser();
    ap_parse(parser, 5, (char *[]){"", "foo", "--", "--bar", "--baz"});
    assert(ap_count_args(parser) == 3);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 7. Commands.
// -----------------------------------------------------------------------------

void test_command(void) {
    ArgParser *parser = ap_new_parser();
    ArgParser *cmd_parser = ap_new_cmd(parser, "cmd");
    ap_add_flag(cmd_parser, "foo");
    ap_add_int_opt(cmd_parser, "bar", 123);
    ap_parse(parser, 7, (char *[]){
        "",
        "cmd",
        "abc", "def",
        "--foo",
        "--bar", "456",
    });
    assert(ap_found_cmd(parser) == true);
    assert(strcmp(ap_get_cmd_name(parser), "cmd") == 0);
    assert(ap_get_cmd_parser(parser) == cmd_parser);
    assert(ap_has_args(cmd_parser) == true);
    assert(ap_count_args(cmd_parser) == 2);
    assert(ap_found(cmd_parser, "foo") == true);
    assert(ap_get_int_value(cmd_parser, "bar") == 456);
    printf(".");
}

// -----------------------------------------------------------------------------
// 8. Miscellanea.
// -----------------------------------------------------------------------------

void test_container_resizing(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_flag(parser, "a alpha");
    ap_add_flag(parser, "b beta");
    ap_add_flag(parser, "c");
    ap_add_flag(parser, "d delta");
    ap_add_flag(parser, "e epsilon");
    ap_add_flag(parser, "f phi");
    ap_add_flag(parser, "g gamma");
    ap_add_flag(parser, "h");
    ap_add_flag(parser, "i iota");
    ap_add_flag(parser, "j");
    ap_add_flag(parser, "k kappa");
    ap_add_flag(parser, "l lambda");
    ap_add_flag(parser, "m mu");
    ap_add_flag(parser, "n nu");
    ap_add_flag(parser, "o omega");
    ap_add_flag(parser, "p pi");
    ap_add_flag(parser, "q");
    ap_add_flag(parser, "r rho");
    ap_add_flag(parser, "s sigma");
    ap_add_flag(parser, "t tau");
    ap_parse(parser, 5, (char *[]){"", "-a", "-b", "-s", "-t"});
    assert(ap_found(parser, "a") == true);
    assert(ap_found(parser, "alpha") == true);
    assert(ap_found(parser, "b") == true);
    assert(ap_found(parser, "beta") == true);
    assert(ap_found(parser, "d") == false);
    assert(ap_found(parser, "delta") == false);
    assert(ap_found(parser, "e") == false);
    assert(ap_found(parser, "epsilon") == false);
    assert(ap_found(parser, "s") == true);
    assert(ap_found(parser, "sigma") == true);
    assert(ap_found(parser, "t") == true);
    assert(ap_found(parser, "tau") == true);
    ap_free(parser);
    printf(".");
}

void test_first_pos_arg_ends_options(void) {
    ArgParser *parser = ap_new_parser();
    ap_first_pos_arg_ends_option_parsing(parser);
    ap_parse(parser, 6, (char *[]){"", "arg", "--foo", "-f", "--bar", "-b"});
    assert(ap_count_args(parser) == 5);
    ap_free(parser);
    printf(".");
}

void test_all_args_as_pos_args(void) {
    ArgParser *parser = ap_new_parser();
    ap_all_args_as_pos_args(parser);
    ap_parse(parser, 6, (char *[]){"", "--foo", "-f", "--bar", "-b", "baz"});
    assert(ap_count_args(parser) == 5);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// 9. Greedy options.
// -----------------------------------------------------------------------------

void test_greedy_str_opt_long(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_greedy_str_opt(parser, "greedy g");
    ap_parse(parser, 8, (char *[]){"", "--greedy", "foo", "--foo", "-f", "bar", "--bar", "-b"});
    assert(ap_found(parser, "greedy") == true);
    assert(ap_count(parser, "greedy") == 6);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 0), "foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 1), "--foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 2), "-f") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 3), "bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 4), "--bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 5), "-b") == 0);
    ap_free(parser);
    printf(".");
}

void test_greedy_str_opt_short(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_greedy_str_opt(parser, "greedy g");
    ap_parse(parser, 8, (char *[]){"", "-g", "foo", "--foo", "-f", "bar", "--bar", "-b"});
    assert(ap_found(parser, "greedy") == true);
    assert(ap_count(parser, "greedy") == 6);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 0), "foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 1), "--foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 2), "-f") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 3), "bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 4), "--bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 5), "-b") == 0);
    ap_free(parser);
    printf(".");
}

void test_greedy_str_opt_long_equals(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_greedy_str_opt(parser, "greedy g");
    ap_parse(parser, 7, (char *[]){"", "--greedy=foo", "--foo", "-f", "bar", "--bar", "-b"});
    assert(ap_found(parser, "greedy") == true);
    assert(ap_count(parser, "greedy") == 6);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 0), "foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 1), "--foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 2), "-f") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 3), "bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 4), "--bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 5), "-b") == 0);
    ap_free(parser);
    printf(".");
}

void test_greedy_str_opt_short_equals(void) {
    ArgParser *parser = ap_new_parser();
    ap_add_greedy_str_opt(parser, "greedy g");
    ap_parse(parser, 7, (char *[]){"", "-g=foo", "--foo", "-f", "bar", "--bar", "-b"});
    assert(ap_found(parser, "greedy") == true);
    assert(ap_count(parser, "greedy") == 6);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 0), "foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 1), "--foo") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 2), "-f") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 3), "bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 4), "--bar") == 0);
    assert(strcmp(ap_get_str_value_at_index(parser, "greedy", 5), "-b") == 0);
    ap_free(parser);
    printf(".");
}

// -----------------------------------------------------------------------------
// Test runner.
// -----------------------------------------------------------------------------

void line(void) {
    for (int i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n");
}

int main(void) {
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
    test_int_opt_multi();

    printf(" 4 ");
    test_dbl_opt_default();
    test_dbl_opt_long();
    test_dbl_opt_short();
    test_dbl_opt_multi();

    printf(" 5 ");
    test_pos_args();
    test_pos_args_as_ints();
    test_pos_args_as_doubles();

    printf(" 6 ");
    test_option_parsing_switch();

    printf(" 7 ");
    test_command();

    printf(" 8 ");
    test_container_resizing();
    test_first_pos_arg_ends_options();
    test_all_args_as_pos_args();

    printf(" 9 ");
    test_greedy_str_opt_long();
    test_greedy_str_opt_short();
    test_greedy_str_opt_long_equals();
    test_greedy_str_opt_short_equals();

    printf(" [ok]\n");
    line();
}
