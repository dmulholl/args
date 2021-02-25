#include "args.h"

int main(int argc, char** argv) {
    // Instantiate a new ArgParser instance.
    ArgParser* parser = ap_new();
    ap_helptext(parser, "Usage: example...");
    ap_version(parser, "1.0");

    // Register a flag and a string-valued option.
    ap_flag(parser, "foo f");
    ap_str_opt(parser, "bar b", "default");

    // Parse the command line arguments.
    ap_parse(parser, argc, argv);
    ap_print(parser);
    ap_free(parser);
}
