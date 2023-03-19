#include <stdio.h>
#include <stdlib.h>
#include "args.h"

int boo_callback(char* cmd_name, ArgParser* cmd_parser) {
    puts("------------- boo! -------------");
    ap_print(cmd_parser);
    puts("--------------------------------\n");
    return 0;
}

int main(int argc, char** argv) {
    // Instantiate a new ArgParser instance.
    ArgParser* parser = ap_new_parser();
    if (!parser) {
        exit(1);
    }

    // Register the program's helptext and version number.
    ap_set_helptext(parser, "Usage: example...");
    ap_set_version(parser, "1.0");

    // Register a command, "boo".
    ArgParser* cmd_parser = ap_new_cmd(parser, "boo");
    if (!cmd_parser) {
        exit(1);
    }

    // The command can have its own helptext, flags, and options.
    ap_set_helptext(cmd_parser, "Usage: example boo...");
    ap_add_flag(cmd_parser, "foo f");
    ap_add_str_opt(cmd_parser, "bar b", "default");

    // The callback function will be called if the command is found.
    ap_set_cmd_callback(cmd_parser, boo_callback);

    // Parse the program's arguments.
    if (!ap_parse(parser, argc, argv)) {
        exit(1);
    }

    // This debugging function prints the parser's state.
    ap_print(parser);

    // We only need to call ap_free() on the root parser.
    ap_free(parser);
}
