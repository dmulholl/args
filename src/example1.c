#include <stdio.h>
#include <stdlib.h>
#include "args.h"

int main(int argc, char** argv) {
    // Instantiate a new ArgParser instance.
    ArgParser* parser = ap_new();
    if (!parser) {
        exit(1);
    }

    // Register the program's helptext and version number.
    ap_set_helptext(parser, "Usage: example...");
    ap_set_version(parser, "1.0");

    // Register a flag and a string-valued option.
    ap_flag(parser, "foo f");
    ap_str_opt(parser, "bar b", "default");

    // Parse the command line arguments.
    if (!ap_parse(parser, argc, argv)) {
        exit(1);
    }

    // This debugging function prints the parser's state.
    ap_print(parser);

    // Free the parser's memory.
    ap_free(parser);
}
