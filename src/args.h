// -----------------------------------------------------------------------------
// Args: a C99 library for parsing command line arguments.
// Version: 2.8.0
// -----------------------------------------------------------------------------

#ifndef args_h
#define args_h

#include <stdbool.h>

// -----------------------------------------------------------------------------
// Types.
// -----------------------------------------------------------------------------

// An ArgParser instance stores registered flags, options and commands.
typedef struct ArgParser ArgParser;

// A command callback function should accept two arguments: the command's name
// and the command's ArgParser instance.
typedef void (*ap_callback_t)(char* cmd_name, ArgParser* cmd_parser);

// -----------------------------------------------------------------------------
// Initialization, parsing, teardown.
// -----------------------------------------------------------------------------

// Initializes a new ArgParser instance. Returns NULL if sufficient memory
// cannot be allocated for the new parser.
ArgParser* ap_new();

// Specifies a helptext string for the parser. If [helptext] is not NULL, this
// activates an automatic --help/-h flag. (Either --help or -h can be overridden
// by explicitly registered flags.) The parser stores and manages its own copy
// of the [helptext] string so [helptext] can be freed immediately after this
// call if it was dynamically constructed.
void ap_set_helptext(ArgParser* parser, const char* helptext);

// Returns a pointer to the parser's helptext string.
char* ap_get_helptext(ArgParser* parser);

// Specifies a version string for the parser. If [version] is not NULL, this
// activates an automatic --version/-v flag. (Either --version or -v can be
// overridden by explicitly registered flags.) The parser stores and manages its
// own copy of the [version] string so [version] can be freed immediately after
// this call if it was dynamically constructed.
void ap_set_version(ArgParser* parser, const char* version);

// Returns a pointer to the parser's version string.
char* ap_get_version(ArgParser* parser);

// If toggled to true, the first positional argument ends option parsing; all
// subsequent arguments will be treated as positionals. Defaults to false.
void ap_first_pos_arg_ends_options(ArgParser* parser, bool enable);

// Parses the application's command line arguments. The parameters are assumed
// to be [argc] and [argv] as supplied to main(), i.e. the first element of the
// array is assumed to be the binary name and is therefore ignored. Returns true
// if the arguments were successfully parsed. Returns false if parsing failed
// due to a memory allocation error. (This memory allocation error may have
// occured during set-up rather than during parsing.)
bool ap_parse(ArgParser* parser, int argc, char** argv);

// Frees the memory associated with the parser and any subparsers.
void ap_free(ArgParser* parser);

// -----------------------------------------------------------------------------
// Register flags and options.
// -----------------------------------------------------------------------------

// Registers a new flag.
void ap_flag(ArgParser* parser, const char* name);

// Registers a new string-valued option.
void ap_str_opt(ArgParser* parser, const char* name, const char* fallback);

// Registers a new integer-valued option.
void ap_int_opt(ArgParser* parser, const char* name, int fallback);

// Registers a new double-valued option.
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

// Returns the string value at the specified index.
char* ap_str_value_at_index(ArgParser* parser, const char* name, int index);

// Returns the value of an integer option.
int ap_int_value(ArgParser* parser, const char* name);

// Returns the integer value at the specified index.
int ap_int_value_at_index(ArgParser* parser, const char* name, int index);

// Returns the value of a floating-point option.
double ap_dbl_value(ArgParser* parser, const char* name);

// Returns the floating-point value at the specified index.
double ap_dbl_value_at_index(ArgParser* parser, const char* name, int index);

// Returns an option's values as a freshly-allocated array of string
// pointers. The array's memory is not affected by calls to ap_free().
// Returns NULL if memory allocation fails.
char** ap_str_values(ArgParser* parser, const char* name);

// Returns an option's values as a freshly-allocated array of integers.
// The array's memory is not affected by calls to ap_free().
// Returns NULL if memory allocation fails.
int* ap_int_values(ArgParser* parser, const char* name);

// Returns an option's values as a freshly-allocated array of doubles.
// The array's memory is not affected by calls to ap_free().
// Returns NULL if memory allocation fails.
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
// calls to ap_free(). Returns NULL if memory allocation fails.
char** ap_args(ArgParser* parser);

// Attempts to parse and return the positional arguments as a freshly allocated
// array of integers. Exits with an error message on failure. The memory
// occupied by the returned array is not affected by calls to ap_free().
// Returns NULL if memory allocation fails.
int* ap_args_as_ints(ArgParser* parser);

// Attempts to parse and return the positional arguments as a freshly allocated
// array of doubles. Exits with an error message on failure. The memory
// occupied by the returned array is not affected by calls to ap_free().
// Returns NULL if memory allocation fails.
double* ap_args_as_doubles(ArgParser* parser);

// -----------------------------------------------------------------------------
// Commands.
// -----------------------------------------------------------------------------

// Registers a new command. Returns the ArgParser instance for the command.
// Returns NULL if sufficient memory cannot be allocated for the new parser.
ArgParser* ap_cmd(ArgParser* parser, const char* name);

// Registers a callback function on a command parser. The function will be
// called if the command is found.
void ap_callback(ArgParser* parser, ap_callback_t function);

// Returns true if the parser has found a command.
bool ap_has_cmd(ArgParser* parser);

// Returns the command name, if the parser has found a command.
char* ap_cmd_name(ArgParser* parser);

// Returns the command's parser instance, if the parser has found a command.
ArgParser* ap_cmd_parser(ArgParser* parser);

// This boolean switch toggles support for an automatic 'help' command that
// prints subcommand helptext. The value defaults to false but gets toggled
// automatically to true whenever a command is registered. You can use this
// function to disable the feature if required.
void ap_enable_help_command(ArgParser* parser, bool enable);

// -----------------------------------------------------------------------------
// Debugging utilities.
// -----------------------------------------------------------------------------

// Dump a parser instance for debugging.
void ap_print(ArgParser* parser);

// True if an attempt to allocate memory failed.
bool ap_had_memory_error(ArgParser* parser);

// -----------------------------------------------------------------------------
// Deprecated.
// -----------------------------------------------------------------------------

// Replaced by ap_enable_help_command().
void ap_cmd_help(ArgParser* parser, bool enable);

// Replaced by ap_set_helptext().
void ap_helptext(ArgParser* parser, const char* helptext);

// Replaced by ap_set_version().
void ap_version(ArgParser* parser, const char* version);

#endif
