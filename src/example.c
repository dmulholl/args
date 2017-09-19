// -----------------------------------------------------------------------------
// A simple application demonstrating Cops in action.
// -----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include "cops.h"


// Callback function for the 'foo' command. The function receives a pointer
// to an ArgParser instance containing the command's parsed arguments. Here
// we simply dump the parser to stdout.
void callback(ArgParser *parser) {
    puts("---------- callback ----------");
    ap_print(parser);
    puts("------------------------------\n");
}


// This sample application will parse its own command-line arguments.
int main(int argc, char **argv) {

    // We instantiate an argument parser, optionally supplying help text and
    // a version string. Supplying help text activates an automatic --help
    // flag, supplying a version string activates an automatic --version
    // flag. We can pass NULL for either parameter.
    ArgParser *parser = ap_new("Help!", "Version 1.2.3");

    // Register a flag, --bool, with a single-character alias, -b. A flag is a
    // boolean option - it's either present (true) or absent (false).
    ap_new_flag(parser, "bool b");

    // Register a string option, --string <arg>, with a single-character
    // alias, -s <arg>. A string option requires a default value, here
    // 'foobar'.
    ap_new_str(parser, "string s", "foobar");

    // Register an integer option, --int <arg>. An integer option requires a
    // default value, here 123.
    ap_new_int(parser, "int", 123);

    // Register a float option, --float <arg>. A float option requires a
    // default value, here 1.0.
    ap_new_double(parser, "float", 1.0);

    // Register a command 'foo'. We need to supply the command's help text and
    // callback function.
    ArgParser *cmd = ap_new_cmd(parser, "foo", "Command!", callback);

    // Registering a command returns a new ArgParser instance dedicated to
    // parsing the command's arguments. We can register as many flags and
    // options as we like on this sub-parser. Note that the sub-parser can
    // reuse the parent's option names without interference.
    ap_new_flag(cmd, "bool b");
    ap_new_int(cmd, "int i", 123);

    // Once all our options and commands have been registered we can call the
    // ap_parse() function with an array of argument strings. (Note that we
    // only need to call ap_parse() on the root parser - command arguments
    // are parsed automatically.)
    ap_parse(parser, argc, argv);

    // We can now retrieve our option and argument values from the parser
    // instance. Here we simply dump the parser to stdout.
    ap_print(parser);

    // We can call ap_free() to free the memory occupied by the parser
    // instance. This will automatically free the memory occupied by
    // any associated command parsers.
    ap_free(parser);
}
