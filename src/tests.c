// -------------------------------------------------------------------------
// Unit tests for the Clio library.
// -------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "clio.h"


// -------------------------------------------------------------------------
// Boolean options.
// -------------------------------------------------------------------------


void test_bool_option_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool");
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_get_flag(parser, "bool") == false);
    ap_free(parser);
    printf(".");
}


void test_bool_option_missing() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool");
    ap_parse(parser, 3, (char *[]){"", "foo", "bar"});
    assert(ap_get_flag(parser, "bool") == false);
    ap_free(parser);
    printf(".");
}


void test_bool_option_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool");
    ap_parse(parser, 2, (char *[]){"", "--bool"});
    assert(ap_get_flag(parser, "bool") == true);
    ap_free(parser);
    printf(".");
}


void test_bool_option_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool b");
    ap_parse(parser, 2, (char *[]){"", "-b"});
    assert(ap_get_flag(parser, "bool") == true);
    ap_free(parser);
    printf(".");
}


// -------------------------------------------------------------------------
// Boolean lists.
// -------------------------------------------------------------------------


void test_bool_list_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag_list(parser, "bool");
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_len_list(parser, "bool") == 0);
    ap_free(parser);
    printf(".");
}


void test_bool_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag_list(parser, "bool");
    ap_parse(parser, 4, (char *[]){"", "--bool", "--bool", "--bool"});
    assert(ap_len_list(parser, "bool") == 3);
    ap_free(parser);
    printf(".");
}


void test_bool_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag_list(parser, "bool");
    ap_parse(parser, 3, (char *[]){"", "-b", "-bb"});
    assert(ap_len_list(parser, "bool") == 3);
    ap_free(parser);
    printf(".");
}


// -------------------------------------------------------------------------
// String options.
// -------------------------------------------------------------------------


void test_string_option_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str(parser, "string", "default");
    ap_parse(parser, 1, (char *[]){""});
    assert(strcmp(ap_get_str(parser, "string"), "default") == 0);
    printf(".");
}


void test_string_option_missing() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str(parser, "string", "default");
    ap_parse(parser, 3, (char *[]){"", "foo", "bar"});
    assert(strcmp(ap_get_str(parser, "string"), "default") == 0);
    printf(".");
}


void test_string_option_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str(parser, "string", "default");
    ap_parse(parser, 3, (char *[]){"", "--string", "value"});
    assert(strcmp(ap_get_str(parser, "string"), "value") == 0);
    printf(".");
}


void test_string_option_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str(parser, "string s", "default");
    ap_parse(parser, 3, (char *[]){"", "-s", "value"});
    assert(strcmp(ap_get_str(parser, "string"), "value") == 0);
    printf(".");
}


// -------------------------------------------------------------------------
// String lists.
// -------------------------------------------------------------------------


void test_string_list_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str_list(parser, "str", false);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_len_list(parser, "str") == 0);
    printf(".");
}


void test_string_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str_list(parser, "str", false);
    ap_parse(parser, 6, (char *[]){"", "--str", "a", "b", "--str", "c"});
    assert(ap_len_list(parser, "str") == 2);
    assert(strcmp(ap_get_str_list(parser, "str")[0], "a") == 0);
    assert(strcmp(ap_get_str_list(parser, "str")[1], "c") == 0);
    printf(".");
}


void test_string_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str_list(parser, "str s", false);
    ap_parse(parser, 6, (char *[]){"", "-s", "a", "b", "-s", "c"});
    assert(ap_len_list(parser, "str") == 2);
    assert(strcmp(ap_get_str_list(parser, "str")[0], "a") == 0);
    assert(strcmp(ap_get_str_list(parser, "str")[1], "c") == 0);
    printf(".");
}


void test_string_greedy_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str_list(parser, "str", true);
    ap_parse(parser, 6, (char *[]){"", "--str", "a", "b", "--str", "c"});
    assert(ap_len_list(parser, "str") == 3);
    assert(strcmp(ap_get_str_list(parser, "str")[0], "a") == 0);
    assert(strcmp(ap_get_str_list(parser, "str")[1], "b") == 0);
    assert(strcmp(ap_get_str_list(parser, "str")[2], "c") == 0);
    printf(".");
}


void test_string_greedy_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_str_list(parser, "str s", true);
    ap_parse(parser, 6, (char *[]){"", "-s", "a", "b", "-s", "c"});
    assert(ap_len_list(parser, "str") == 3);
    assert(strcmp(ap_get_str_list(parser, "str")[0], "a") == 0);
    assert(strcmp(ap_get_str_list(parser, "str")[1], "b") == 0);
    assert(strcmp(ap_get_str_list(parser, "str")[2], "c") == 0);
    printf(".");
}


// -------------------------------------------------------------------------
// Integer options.
// -------------------------------------------------------------------------


void test_int_option_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int(parser, "int", 101);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_get_int(parser, "int") == 101);
    printf(".");
}


void test_int_option_missing() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int(parser, "int", 101);
    ap_parse(parser, 3, (char *[]){"", "foo", "bar"});
    assert(ap_get_int(parser, "int") == 101);
    printf(".");
}


void test_int_option_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int(parser, "int", 101);
    ap_parse(parser, 3, (char *[]){"", "--int", "202"});
    assert(ap_get_int(parser, "int") == 202);
    printf(".");
}


void test_int_option_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int(parser, "int i", 101);
    ap_parse(parser, 3, (char *[]){"", "-i", "202"});
    assert(ap_get_int(parser, "int") == 202);
    printf(".");
}


// -------------------------------------------------------------------------
// Integer lists.
// -------------------------------------------------------------------------


void test_int_list_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int_list(parser, "int", false);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_len_list(parser, "int") == 0);
    printf(".");
}


void test_int_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int_list(parser, "int", false);
    ap_parse(parser, 6, (char *[]){"", "--int", "1", "2", "--int", "3"});
    assert(ap_len_list(parser, "int") == 2);
    assert(ap_get_int_list(parser, "int")[0] == 1);
    assert(ap_get_int_list(parser, "int")[1] == 3);
    printf(".");
}


void test_int_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int_list(parser, "int i", false);
    ap_parse(parser, 6, (char *[]){"", "-i", "1", "2", "-i", "3"});
    assert(ap_len_list(parser, "int") == 2);
    assert(ap_get_int_list(parser, "int")[0] == 1);
    assert(ap_get_int_list(parser, "int")[1] == 3);
    printf(".");
}


void test_int_greedy_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int_list(parser, "int", true);
    ap_parse(parser, 6, (char *[]){"", "--int", "1", "2", "--int", "3"});
    assert(ap_len_list(parser, "int") == 3);
    assert(ap_get_int_list(parser, "int")[0] == 1);
    assert(ap_get_int_list(parser, "int")[1] == 2);
    assert(ap_get_int_list(parser, "int")[2] == 3);
    printf(".");
}


void test_int_greedy_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_int_list(parser, "int i", true);
    ap_parse(parser, 6, (char *[]){"", "-i", "1", "2", "-i", "3"});
    assert(ap_len_list(parser, "int") == 3);
    assert(ap_get_int_list(parser, "int")[0] == 1);
    assert(ap_get_int_list(parser, "int")[1] == 2);
    assert(ap_get_int_list(parser, "int")[2] == 3);
    printf(".");
}


// -------------------------------------------------------------------------
// Float options.
// -------------------------------------------------------------------------


void test_float_option_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float(parser, "float", 1.1);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_get_float(parser, "float") == 1.1);
    printf(".");
}


void test_float_option_missing() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float(parser, "float", 1.1);
    ap_parse(parser, 3, (char *[]){"", "foo", "bar"});
    assert(ap_get_float(parser, "float") == 1.1);
    printf(".");
}


void test_float_option_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float(parser, "float", 1.1);
    ap_parse(parser, 3, (char *[]){"", "--float", "2.2"});
    assert(ap_get_float(parser, "float") == 2.2);
    printf(".");
}


void test_float_option_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float(parser, "float f", 1.1);
    ap_parse(parser, 3, (char *[]){"", "-f", "2.2"});
    assert(ap_get_float(parser, "float") == 2.2);
    printf(".");
}


// -------------------------------------------------------------------------
// Float lists.
// -------------------------------------------------------------------------


void test_float_list_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float_list(parser, "float", false);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_len_list(parser, "float") == 0);
    printf(".");
}


void test_float_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float_list(parser, "flt", false);
    ap_parse(parser, 6, (char *[]){"", "--flt", "1", "2", "--flt", "3"});
    assert(ap_len_list(parser, "flt") == 2);
    assert(ap_get_float_list(parser, "flt")[0] == 1.0);
    assert(ap_get_float_list(parser, "flt")[1] == 3.0);
    printf(".");
}


void test_float_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float_list(parser, "flt f", false);
    ap_parse(parser, 6, (char *[]){"", "-f", "1", "2", "-f", "3"});
    assert(ap_len_list(parser, "flt") == 2);
    assert(ap_get_float_list(parser, "flt")[0] == 1.0);
    assert(ap_get_float_list(parser, "flt")[1] == 3.0);
    printf(".");
}


void test_float_greedy_list_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float_list(parser, "flt", true);
    ap_parse(parser, 6, (char *[]){"", "--flt", "1", "2", "--flt", "3"});
    assert(ap_len_list(parser, "flt") == 3);
    assert(ap_get_float_list(parser, "flt")[0] == 1.0);
    assert(ap_get_float_list(parser, "flt")[1] == 2.0);
    assert(ap_get_float_list(parser, "flt")[2] == 3.0);
    printf(".");
}


void test_float_greedy_list_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_float_list(parser, "flt f", true);
    ap_parse(parser, 6, (char *[]){"", "-f", "1", "2", "-f", "3"});
    assert(ap_len_list(parser, "flt") == 3);
    assert(ap_get_float_list(parser, "flt")[0] == 1.0);
    assert(ap_get_float_list(parser, "flt")[1] == 2.0);
    assert(ap_get_float_list(parser, "flt")[2] == 3.0);
    printf(".");
}


// -------------------------------------------------------------------------
// Multiple options.
// -------------------------------------------------------------------------


void test_multi_options_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool1");
    ap_new_flag(parser, "bool2 b");
    ap_new_str(parser, "string1", "default1");
    ap_new_str(parser, "string2 s", "default2");
    ap_new_int(parser, "int1", 101);
    ap_new_int(parser, "int2 i", 202);
    ap_new_float(parser, "float1", 1.1);
    ap_new_float(parser, "float2 f", 2.2);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_get_flag(parser, "bool1") == false);
    assert(ap_get_flag(parser, "bool2") == false);
    assert(strcmp(ap_get_str(parser, "string1"), "default1") == 0);
    assert(strcmp(ap_get_str(parser, "string2"), "default2") == 0);
    assert(ap_get_int(parser, "int1") == 101);
    assert(ap_get_int(parser, "int2") == 202);
    assert(ap_get_float(parser, "float1") == 1.1);
    assert(ap_get_float(parser, "float2") == 2.2);
    printf(".");
}


void test_multi_options_longform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool1");
    ap_new_flag(parser, "bool2 b");
    ap_new_str(parser, "string1", "default1");
    ap_new_str(parser, "string2 s", "default2");
    ap_new_int(parser, "int1", 101);
    ap_new_int(parser, "int2 i", 202);
    ap_new_float(parser, "float1", 1.1);
    ap_new_float(parser, "float2 f", 2.2);
    ap_parse(parser, 15, (char *[]){
        "",
        "--bool1",
        "--bool2",
        "--string1", "value1",
        "--string2", "value2",
        "--int1", "303",
        "--int2", "404",
        "--float1", "3.3",
        "--float2", "4.4",
    });
    assert(ap_get_flag(parser, "bool1") == true);
    assert(ap_get_flag(parser, "bool2") == true);
    assert(strcmp(ap_get_str(parser, "string1"), "value1") == 0);
    assert(strcmp(ap_get_str(parser, "string2"), "value2") == 0);
    assert(ap_get_int(parser, "int1") == 303);
    assert(ap_get_int(parser, "int2") == 404);
    assert(ap_get_float(parser, "float1") == 3.3);
    assert(ap_get_float(parser, "float2") == 4.4);
    printf(".");
}


void test_multi_options_shortform() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool1");
    ap_new_flag(parser, "bool2 b");
    ap_new_str(parser, "string1", "default1");
    ap_new_str(parser, "string2 s", "default2");
    ap_new_int(parser, "int1", 101);
    ap_new_int(parser, "int2 i", 202);
    ap_new_float(parser, "float1", 1.1);
    ap_new_float(parser, "float2 f", 2.2);
    ap_parse(parser, 15, (char *[]){
        "",
        "--bool1",
        "-b",
        "--string1", "value1",
        "-s", "value2",
        "--int1", "303",
        "-i", "404",
        "--float1", "3.3",
        "-f", "4.4",
    });
    assert(ap_get_flag(parser, "bool1") == true);
    assert(ap_get_flag(parser, "bool2") == true);
    assert(strcmp(ap_get_str(parser, "string1"), "value1") == 0);
    assert(strcmp(ap_get_str(parser, "string2"), "value2") == 0);
    assert(ap_get_int(parser, "int1") == 303);
    assert(ap_get_int(parser, "int2") == 404);
    assert(ap_get_float(parser, "float1") == 3.3);
    assert(ap_get_float(parser, "float2") == 4.4);
    printf(".");
}


// -------------------------------------------------------------------------
// Condensed short-form options.
// -------------------------------------------------------------------------


void test_condensed_options() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_flag(parser, "bool b");
    ap_new_str(parser, "string s", "default");
    ap_new_int(parser, "int i", 101);
    ap_new_float(parser, "float f", 1.1);
    ap_parse(parser, 5, (char *[]){"", "-bsif", "value", "202", "2.2"});
    assert(ap_get_flag(parser, "bool") == true);
    assert(strcmp(ap_get_str(parser, "string"), "value") == 0);
    assert(ap_get_int(parser, "int") == 202);
    assert(ap_get_float(parser, "float") == 2.2);
    printf(".");
}


// -------------------------------------------------------------------------
// Positional arguments.
// -------------------------------------------------------------------------


void test_positional_args_empty() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_has_args(parser) == false);
    printf(".");
}


void test_positional_args() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_parse(parser, 3, (char *[]){"", "foo", "bar"});
    assert(ap_has_args(parser) == true);
    assert(ap_num_args(parser) == 2);
    assert(strcmp(ap_get_args(parser)[0], "foo") == 0);
    assert(strcmp(ap_get_args(parser)[1], "bar") == 0);
    printf(".");
}


void test_positional_args_as_ints() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_parse(parser, 3, (char *[]){"", "1", "11"});
    assert(ap_get_args_as_ints(parser)[0] == 1);
    assert(ap_get_args_as_ints(parser)[1] == 11);
    printf(".");
}


void test_positional_args_as_floats() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_parse(parser, 3, (char *[]){"", "1.1", "11.1"});
    assert(ap_get_args_as_floats(parser)[0] == 1.1);
    assert(ap_get_args_as_floats(parser)[1] == 11.1);
    printf(".");
}


// -------------------------------------------------------------------------
// Option parsing switch.
// -------------------------------------------------------------------------


void test_option_parsing_switch() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_parse(parser, 5, (char *[]){"", "foo", "--", "--bar", "--baz"});
    assert(ap_num_args(parser) == 3);
    printf(".");
}


// -------------------------------------------------------------------------
// Commands.
// -------------------------------------------------------------------------


void callback(ArgParser *parser) {}


void test_command_absent() {
    ArgParser *parser = ap_new(NULL, NULL);
    ap_new_cmd(parser, "cmd", "helptext", callback);
    ap_parse(parser, 1, (char *[]){""});
    assert(ap_has_cmd(parser) == false);
    printf(".");
}


void test_command_present() {
    ArgParser *parser = ap_new(NULL, NULL);
    ArgParser *cmd_parser = ap_new_cmd(parser, "cmd", "helptext", callback);
    ap_parse(parser, 2, (char *[]){"", "cmd"});
    assert(ap_has_cmd(parser) == true);
    assert(strcmp(ap_get_cmd_name(parser), "cmd") == 0);
    assert(ap_get_cmd_parser(parser) == cmd_parser);
    printf(".");
}


void test_command_with_options() {
    ArgParser *parser = ap_new(NULL, NULL);
    ArgParser *cmd_parser = ap_new_cmd(parser, "cmd", "helptext", callback);
    ap_new_flag(cmd_parser, "bool");
    ap_new_str(cmd_parser, "string", "default");
    ap_new_int(cmd_parser, "int", 101);
    ap_new_float(cmd_parser, "float", 1.1);
    ap_parse(parser, 10, (char *[]){
        "",
        "cmd",
        "foo", "bar",
        "--string", "value",
        "--int", "202",
        "--float", "2.2",
    });
    assert(ap_has_cmd(parser) == true);
    assert(strcmp(ap_get_cmd_name(parser), "cmd") == 0);
    assert(ap_get_cmd_parser(parser) == cmd_parser);
    assert(ap_has_args(cmd_parser) == true);
    assert(ap_num_args(cmd_parser) == 2);
    assert(strcmp(ap_get_str(cmd_parser, "string"), "value") == 0);
    assert(ap_get_int(cmd_parser, "int") == 202);
    assert(ap_get_float(cmd_parser, "float") == 2.2);
    printf(".");
}


// -------------------------------------------------------------------------
// Test runner.
// -------------------------------------------------------------------------


void line() {
    for (int i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n");
}


int main() {
    setbuf(stdout, NULL);

    line();
    printf("Tests: ");

    test_bool_option_empty();
    test_bool_option_missing();
    test_bool_option_longform();
    test_bool_option_shortform();
    test_bool_list_empty();
    test_bool_list_longform();
    test_bool_option_shortform();

    test_string_option_empty();
    test_string_option_missing();
    test_string_option_longform();
    test_string_option_shortform();
    test_string_list_empty();
    test_string_list_longform();
    test_string_list_shortform();
    test_string_greedy_list_longform();
    test_string_greedy_list_shortform();

    test_int_option_empty();
    test_int_option_missing();
    test_int_option_longform();
    test_int_option_shortform();
    test_int_list_empty();
    test_int_list_longform();
    test_int_list_shortform();
    test_int_greedy_list_longform();
    test_int_greedy_list_shortform();

    test_float_option_empty();
    test_float_option_missing();
    test_float_option_longform();
    test_float_option_shortform();
    test_float_list_empty();
    test_float_list_longform();
    test_float_list_shortform();
    test_float_greedy_list_longform();
    test_float_greedy_list_shortform();

    test_multi_options_empty();
    test_multi_options_longform();
    test_multi_options_shortform();

    test_condensed_options();

    test_positional_args_empty();
    test_positional_args();
    test_positional_args_as_ints();
    test_positional_args_as_floats();

    test_option_parsing_switch();

    test_command_absent();
    test_command_present();
    test_command_with_options();

    printf(" [ok]\n");
    line();
}
