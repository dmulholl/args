#include <stdio.h>
#include "args.h"

void cmd_boo(char *cmd_name, ArgParser *cmd_parser) {
    puts("------------- boo! -------------");
    ap_print(cmd_parser);
    puts("--------------------------------\n");
}

int main(int argc, char **argv) {
    // Instantiate a new ArgParser instance.
    ArgParser *parser = ap_new();
    ap_helptext(parser, "Usage: example...");
    ap_version(parser, "1.0");

    // Register a command, "boo". The command can have its own helptext, flags, and options.
    ArgParser *cmd_parser = ap_cmd(parser, "boo");
    ap_helptext(cmd_parser, "Usage: example boo...");
    ap_flag(cmd_parser, "foo f");
    ap_str_opt(cmd_parser, "bar b", "default");

    // The callback function will be called if the command is found.
    ap_callback(cmd_parser, cmd_boo);

    // Parse the program's arguments.
    ap_parse(parser, argc, argv);
    ap_print(parser);
    ap_free(parser);
}
