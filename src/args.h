// -----------------------------------------------------------------------------
// Args: a C99 library for parsing command line arguments.
// Version: 2.1.3
// -----------------------------------------------------------------------------

#ifndef args_h
#define args_h

#include <stdbool.h>

// -----------------------------------------------------------------------------
// Types.
// -----------------------------------------------------------------------------

// An ArgParser instance stores registered flags, options and commands.
typedef struct ArgParser ArgParser;

// -----------------------------------------------------------------------------
// Initialization, parsing, teardown.
// -----------------------------------------------------------------------------

// Initialize a new ArgParser instance.
ArgParser* ap_new();

// Supplying a helptext string activates an automatic --help/-h flag.
void ap_helptext(ArgParser* parser, const char* helptext);

// Supplying a version string activates an automatic --version/-v flag.
void ap_version(ArgParser* parser, const char* version);

// Parse the application's command line arguments. The parameters are assumed
// to be argc and argv as supplied to main().
void ap_parse(ArgParser* parser, int argc, char** argv);

// Free the memory associated with an ArgParser instance.
void ap_free(ArgParser* parser);

// -----------------------------------------------------------------------------
// Register flags and options.
// -----------------------------------------------------------------------------

// Register a new flag.
void ap_flag(ArgParser* parser, const char* name);

// Register a new string-valued option.
void ap_str_opt(ArgParser* parser, const char* name, const char* fallback);

// Register a new integer-valued option.
void ap_int_opt(ArgParser* parser, const char* name, int fallback);

// Register a new double-valued option.
void ap_dbl_opt(ArgParser* parser, const char* name, double fallback);

// -----------------------------------------------------------------------------
// Inspect flags and options.
// -----------------------------------------------------------------------------

// Returns the number of times the specified flag or option was found.
int ap_count(ArgParser* parser, const char* name);

// Returns true if the specified flag or option was found.
bool ap_found(ArgParser* parser, const char* name);

// Returns the value of a string option.
char* ap_str_value(ArgParser* parser, const char* name);

// Returns the value of an integer option.
int ap_int_value(ArgParser* parser, const char* name);

// Returns the value of a floating-point option.
double ap_dbl_value(ArgParser* parser, const char* name);

// Returns an option's values as a freshly-allocated array of string
// pointers. The array's memory is not affected by calls to ap_free().
char** ap_str_values(ArgParser* parser, const char* name);

// Returns an option's values as a freshly-allocated array of integers.
// The array's memory is not affected by calls to ap_free().
int* ap_int_values(ArgParser* parser, const char* name);

// Returns an option's values as a freshly-allocated array of doubles.
// The array's memory is not affected by calls to ap_free().
double* ap_dbl_values(ArgParser* parser, const char* name);

// -----------------------------------------------------------------------------
// Positional arguments.
// -----------------------------------------------------------------------------

// Returns true if the parser has found one or more positional arguments.
bool ap_has_args(ArgParser* parser);

// Returns the number of positional arguments.
int ap_count_args(ArgParser* parser);

// Returns the positional argument at the specified index.
char* ap_arg(ArgParser* parser, int index);

// Returns the positional arguments as a freshly-allocated array of string
// pointers. The memory occupied by the returned array is not affected by
// calls to ap_free().
char** ap_args(ArgParser* parser);

// Attempts to parse and return the positional arguments as a freshly allocated
// array of integers. Exits with an error message on failure. The memory
// occupied by the returned array is not affected by calls to ap_free().
int* ap_args_as_ints(ArgParser* parser);

// Attempts to parse and return the positional arguments as a freshly allocated
// array of doubles. Exits with an error message on failure. The memory
// occupied by the returned array is not affected by calls to ap_free().
double* ap_args_as_doubles(ArgParser* parser);

// -----------------------------------------------------------------------------
// Commands.
// -----------------------------------------------------------------------------

// Registers a new command. Returns the ArgParser instance for the command.
ArgParser* ap_cmd(ArgParser* parser, const char* name);

// Registers a callback function for a command. The callback is registered on
// the command's ArgParser instance. The callback should accept two arguments:
// the command's name and the command's ArgParser instance.
void ap_callback(
    ArgParser* parser,
    void (*callback)(char* cmd_name, ArgParser* cmd_parser)
);

// Returns true if the parser has found a command.
bool ap_has_cmd(ArgParser* parser);

// Returns the command name, if the parser has found a command.
char* ap_cmd_name(ArgParser* parser);

// Returns the command's parser instance, if the parser has found a command.
ArgParser* ap_cmd_parser(ArgParser* parser);

// Boolean switch; toggles support for an automatic 'help' command which prints
// subcommand helptext. (Defaults to false; gets toggled automatically to true
// when a command is registered.)
void ap_cmd_help(ArgParser* parser, bool enable);

// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------

// Dump a parser instance for debugging.
void ap_print(ArgParser* parser);

#endif
