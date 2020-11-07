// -----------------------------------------------------------------------------
// Args: a minimalist C99 library for parsing command line arguments.
// -----------------------------------------------------------------------------

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>


// -----------------------------------------------------------------------------
// Utility functions.
// -----------------------------------------------------------------------------


// Print a message to stderr and exit with a non-zero error code.
static void err(const char *msg) {
    fprintf(stderr, "Error: %s.\n", msg);
    exit(1);
}


// Print to an automatically-allocated string. Returns NULL if an encoding
// error occurs or if sufficient memory cannot be allocated.
static char* str(const char *fmtstr, ...) {
    va_list args;

    va_start(args, fmtstr);
    int len = vsnprintf(NULL, 0, fmtstr, args);
    if (len < 1) {
        return NULL;
    }
    va_end(args);

    char *string = malloc(len + 1);
    if (string == NULL) {
        return NULL;
    }

    va_start(args, fmtstr);
    vsnprintf(string, len + 1, fmtstr, args);
    va_end(args);

    return string;
}


// Duplicate a string, automatically allocating memory for the copy.
static char* str_dup(const char *arg) {
    size_t len = strlen(arg) + 1;
    char *copy = malloc(len);
    return copy ? memcpy(copy, arg, len) : NULL;
}


// Attempt to parse a string as an integer value, exiting on failure.
static int try_str_to_int(const char *arg) {
    char *endptr;
    errno = 0;
    long result = strtol(arg, &endptr, 0);
    if (errno == ERANGE || result > INT_MAX || result < INT_MIN) {
        err(str("'%s' is out of range", arg));
    }
    if (*endptr != '\0') {
        err(str("cannot parse '%s' as an integer", arg));
    }
    return (int) result;
}


// Attempt to parse a string as a double value, exiting on failure.
static double try_str_to_double(const char *arg) {
    char *endptr;
    errno = 0;
    double result = strtod(arg, &endptr);
    if (errno == ERANGE) {
        err(str("'%s' is out of range", arg));
    }
    if (*endptr != '\0') {
        err(str("cannot parse '%s' as a floating-point value", arg));
    }
    return result;
}


// -----------------------------------------------------------------------------
// Map: a simple container mapping string keys to pointer values.
// -----------------------------------------------------------------------------


// Callback type for freeing a value stored in a Map instance.
typedef void (*MapFreeCB)(void *value);


// Stores a map entry as a key-value pair.
typedef struct MapEntry {
    char *key;
    void *value;
} MapEntry;


// A Map instance maps string keys to pointer values.
typedef struct Map {
    int len;
    int cap;
    MapEntry *entries;
    MapFreeCB free_cb;
} Map;


// Free the memory occupied by a Map instance and all its values.
static void map_free(Map *map) {

    // Maintain a list of freed values so we don't attempt to free the same
    // value twice.
    void **freed_values = malloc(sizeof(void*) * map->len);
    int len_freed_values = 0;

    // Loop over all the entries in the map.
    for (int i = 0; i < map->len; i++) {
        char *key = map->entries[i].key;
        void *value = map->entries[i].value;

        // Free the memory occupied by the entry's key.
        free(key);

        // Has this entry's value been freed already?
        bool freed = false;
        for (int j = 0; j < len_freed_values; j++) {
            if (freed_values[j] == value) {
                freed = true;
                break;
            }
        }

        // Free the memory occupied by the entry's value.
        if (!freed) {
            freed_values[len_freed_values++] = value;
            if (map->free_cb != NULL) {
                map->free_cb(value);
            }
        }
    }

    free(freed_values);
    free(map->entries);
    free(map);
}


// Initialize a new Map instance.
static Map* map_new(MapFreeCB callback) {
    Map *map = malloc(sizeof(Map));
    map->len = 0;
    map->cap = 10;
    map->entries = malloc(sizeof(MapEntry) * map->cap);
    map->free_cb = callback;
    return map;
}


// Add a key-value pair to a map.
static void map_add(Map *map, const char *key, void *value) {

    // Do we need to increase the map's capacity?
    if (map->len == map->cap) {
        map->cap *= 2;
        map->entries = realloc(map->entries, sizeof(MapEntry) * map->cap);
    }

    // Make a copy of the key.
    char *copiedkey = str_dup(key);

    // Add a MapEntry instance to the map's entry list.
    map->entries[map->len++] = (MapEntry){.key = copiedkey, .value = value};
}


// Split the specified keystring on spaces into multiple keys and add a
// separate key-value pair to the map for each key.
static void map_add_splitkey(Map *map, const char *keystr, void *value) {
    char *key;
    char *saveptr;
    char *keystr_cpy = str_dup(keystr);

    key = strtok_r(keystr_cpy, " ", &saveptr);
    while (key != NULL) {
        map_add(map, key, value);
        key = strtok_r(NULL, " ", &saveptr);
    }

    free(keystr_cpy);
}


// Test if a Map instance contains the specified key.
static bool map_contains(Map *map, const char *key) {
    for (int i = 0; i < map->len; i++) {
        if (strcmp(key, map->entries[i].key) == 0) {
            return true;
        }
    }
    return false;
}


// Retrieve a value from a Map instance. Returns NULL if the key is not found.
static void* map_get(Map *map, const char *key) {
    for (int i = 0; i < map->len; i++) {
        if (strcmp(key, map->entries[i].key) == 0) {
            return map->entries[i].value;
        }
    }
    return NULL;
}


// Returns the key at the specified index. (Only used in the print function.)
static char* map_key_at_index(Map *map, int i) {
    return map->entries[i].key;
}


// Returns the value at the specified index. (Only used in the print function.)
static void* map_value_at_index(Map *map, int i) {
    return map->entries[i].value;
}


// -----------------------------------------------------------------------------
// Options.
// -----------------------------------------------------------------------------


// Valid option types.
typedef enum OptionType {
    FLAG,
    STROPT,
    INTOPT,
    DBLOPT,
} OptionType;


// Union combining all three valid types of option value.
typedef union OptionValue {
    char *str_val;
    int int_val;
    double dbl_val;
} OptionValue;


// An Option instance represents an option registered on a parser.
typedef struct Option {
    OptionType type;
    int count;
    int capacity;
    OptionValue *values;
    OptionValue fallback;
} Option;


// Free the memory occupied by an Option instance.
static void option_free(Option *opt) {
    free(opt->values);
    free(opt);
}


// Callback for freeing an Option instance stored in a Map.
static void option_free_cb(void *opt) {
    option_free((Option*)opt);
}


// Append a value to an Option instance's internal list of values.
static void option_append(Option *opt, OptionValue value) {
    if (opt->capacity == 0) {
        opt->capacity = 1;
        opt->values = malloc(sizeof(OptionValue));
    } else if (opt->count == opt->capacity) {
        opt->capacity *= 2;
        opt->values = realloc(opt->values, sizeof(OptionValue) * opt->capacity);
    }
    opt->values[opt->count] = value;
    opt->count++;
}


// Append a value to a string option's internal list.
static void option_append_str(Option *opt, char *value) {
    option_append(opt, (OptionValue){.str_val = value});
}


// Append a value to an integer option's internal list.
static void option_append_int(Option *opt, int value) {
    option_append(opt, (OptionValue){.int_val = value});
}


// Append a value to a floating-point option's internal list.
static void option_append_double(Option *opt, double value) {
    option_append(opt, (OptionValue){.dbl_val = value});
}


// Try setting an option by parsing the value of a string argument. Exits with
// an error message on failure.
static void option_try_set(Option *opt, char *arg) {
    if (opt->type == STROPT) {
        option_append_str(opt, arg);
    }
    else if (opt->type == INTOPT) {
        option_append_int(opt, try_str_to_int(arg));
    }
    else if (opt->type == DBLOPT) {
        option_append_double(opt, try_str_to_double(arg));
    }
}


// Initialize a new Option instance.
static Option* option_new() {
    Option *option = malloc(sizeof(Option));
    option->count = 0;
    option->capacity = 0;
    option->values = NULL;
    return option;
}


// Initialize a new flag.
static Option* option_new_flag() {
    Option *opt = option_new();
    opt->type = FLAG;
    return opt;
}


// Initialize a new string-valued option.
static Option* option_new_str(char *fallback) {
    Option *opt = option_new();
    opt->type = STROPT;
    opt->fallback = (OptionValue){.str_val = fallback};
    return opt;
}


// Initialize a new integer-valued option.
static Option* option_new_int(int fallback) {
    Option *opt = option_new();
    opt->type = INTOPT;
    opt->fallback = (OptionValue){.int_val = fallback};
    return opt;
}


// Initialize a new double-valued option.
static Option* option_new_double(double fallback) {
    Option *opt = option_new();
    opt->type = DBLOPT;
    opt->fallback = (OptionValue){.dbl_val = fallback};
    return opt;
}


// Returns the value of a string option.
static char* option_get_str(Option *opt) {
    if (opt->count > 0) {
        return opt->values[opt->count - 1].str_val;
    }
    return opt->fallback.str_val;
}


// Returns the value of an integer option.
static int option_get_int(Option *opt) {
    if (opt->count > 0) {
        return opt->values[opt->count - 1].int_val;
    }
    return opt->fallback.int_val;
}


// Returns the value of a floating-point option.
static double option_get_double(Option *opt) {
    if (opt->count > 0) {
        return opt->values[opt->count - 1].dbl_val;
    }
    return opt->fallback.dbl_val;
}


// Returns an option's values as a freshly-allocated array of strings.
static char** option_get_str_list(Option *opt) {
    if (opt->count == 0) {
        return NULL;
    }
    char **list = malloc(sizeof(char*) * opt->count);
    for (int i = 0; i < opt->count; i++) {
        list[i] = opt->values[i].str_val;
    }
    return list;
}


// Returns an option's values as a freshly-allocated array of integers.
static int* option_get_int_list(Option *opt) {
    if (opt->count == 0) {
        return NULL;
    }
    int *list = malloc(sizeof(int) * opt->count );
    for (int i = 0; i < opt->count ; i++) {
        list[i] = opt->values[i].int_val;
    }
    return list;
}


// Returns an option's values as a freshly-allocated array of doubles.
static double* option_get_double_list(Option *opt) {
    if (opt->count  == 0) {
        return NULL;
    }
    double *list = malloc(sizeof(double) * opt->count );
    for (int i = 0; i < opt->count ; i++) {
        list[i] = opt->values[i].dbl_val;
    }
    return list;
}


// Returns a freshly-allocated state-string for debugging.
static char* option_to_str(Option *opt) {
    if (opt->type == FLAG) {
        return str("%i", opt->count);
    }

    char *fallback = NULL;
    if (opt->type == STROPT) {
        fallback = str_dup(opt->fallback.str_val);
    } else if (opt->type == INTOPT) {
        fallback = str("%i", opt->fallback.int_val);
    } else if (opt->type == DBLOPT) {
        fallback = str("%f", opt->fallback.dbl_val);
    }

    char *values = str_dup("");
    for (int i = 0; i < opt->count; i++) {
        char *value = NULL;
        if (opt->type == STROPT) {
            value = str_dup(opt->values[i].str_val);
        } else if (opt->type == INTOPT) {
            value = str("%i", opt->values[i].int_val);
        } else if (opt->type == DBLOPT) {
            value = str("%f", opt->values[i].dbl_val);
        }
        char *old_values = values;
        if (i == 0) {
            values = str_dup(value);
        } else {
            values = str("%s, %s", old_values, value);
        }
        free(old_values);
        free(value);
    }

    char *output = str("(%s) [%s]", fallback, values);
    free(fallback);
    free(values);
    return output;
}


// -----------------------------------------------------------------------------
// ArgStream
// -----------------------------------------------------------------------------


// An ArgStream instance is a wrapper for an array of string pointers,
// allowing it to be accessed as a stream.
typedef struct ArgStream {
    int len;
    int index;
    char **args;
} ArgStream;


// Free the memory associated with an ArgStream instance.
static void argstream_free(ArgStream *stream) {
    free(stream);
}


// Initialize a new ArgStream instance.
static ArgStream* argstream_new(int len, char **args) {
    ArgStream *stream = malloc(sizeof(ArgStream));
    stream->len = len;
    stream->index = 0;
    stream->args = args;
    return stream;
}


// Returns the next argument from the stream.
static char* argstream_next(ArgStream *stream) {
    return stream->args[stream->index++];
}


// Returns true if the stream contains at least one more element.
static bool argstream_has_next(ArgStream *stream) {
    return stream->index < stream->len;
}


// -----------------------------------------------------------------------------
// ArgList
// -----------------------------------------------------------------------------


// Container for storing positional arguments parsed from the input stream.
typedef struct ArgList {
    int len;
    int cap;
    char **args;
} ArgList;


// Free the memory associated with an ArgList instance.
static void arglist_free(ArgList *list) {
    free(list->args);
    free(list);
}


// Initialize a new ArgList instance.
static ArgList* arglist_new() {
    ArgList *list = malloc(sizeof(ArgList));
    list->len = 0;
    list->cap = 10;
    list->args = malloc(sizeof(char*) * list->cap);
    return list;
}


// Append an argument to the list.
static void arglist_append(ArgList *list, char *arg) {
    if (list->len == list->cap) {
        list->cap *= 2;
        list->args = realloc(list->args, sizeof(char*) * list->cap);
    }
    list->args[list->len] = arg;
    list->len++;
}


// Print the ArgList instance to stdout.
static void arglist_print(ArgList *list) {
    puts("Arguments:");
    if (list->len > 0) {
        for (int i = 0; i < list->len; i++) {
            printf("  %s\n", list->args[i]);
        }
    } else {
        puts("  [none]");
    }
}


// -----------------------------------------------------------------------------
// ArgParser: setup.
// -----------------------------------------------------------------------------


// An ArgParser instance stores registered flags, options and commands.
typedef struct ArgParser {
    char *helptext;
    char *version;
    Map *options;
    Map *commands;
    ArgList *arguments;
    void (*callback)(char *cmd_name, struct ArgParser *cmd_parser);
    char *cmd_name;
    struct ArgParser *cmd_parser;
    bool cmd_help;
} ArgParser;


// Free the memory associated with an ArgParser instance.
void ap_free(ArgParser *parser) {
    map_free(parser->options);
    map_free(parser->commands);
    arglist_free(parser->arguments);
    free(parser);
}


// Callback for freeing an ArgParser instance stored in a Map.
static void ap_free_cb(void *parser) {
    ap_free((ArgParser*)parser);
}


// Initialize a new ArgParser instance.
ArgParser* ap_new() {
    ArgParser *parser = malloc(sizeof(ArgParser));
    parser->helptext = NULL;
    parser->version = NULL;
    parser->options = map_new(option_free_cb);
    parser->commands = map_new(ap_free_cb);
    parser->arguments = arglist_new();
    parser->callback = NULL;
    parser->cmd_name = NULL;
    parser->cmd_parser = NULL;
    parser->cmd_help = false;
    return parser;
}


// Specify a helptext string.
void ap_helptext(ArgParser *parser, char *helptext) {
    parser->helptext = helptext;
}


// Specify a version string.
void ap_version(ArgParser *parser, char *version) {
    parser->version = version;
}


// -----------------------------------------------------------------------------
// ArgParser: register flags and options.
// -----------------------------------------------------------------------------


// Register a new flag.
void ap_flag(ArgParser *parser, const char *name) {
    Option *opt = option_new_flag();
    map_add_splitkey(parser->options, name, opt);
}


// Register a new string-valued option.
void ap_str_opt(ArgParser *parser, const char *name, char* fallback) {
    Option *opt = option_new_str(fallback);
    map_add_splitkey(parser->options, name, opt);
}


// Register a new integer-valued option.
void ap_int_opt(ArgParser *parser, const char *name, int fallback) {
    Option *opt = option_new_int(fallback);
    map_add_splitkey(parser->options, name, opt);
}


// Register a new double-valued option.
void ap_dbl_opt(ArgParser *parser, const char *name, double fallback) {
    Option *opt = option_new_double(fallback);
    map_add_splitkey(parser->options, name, opt);
}


// -----------------------------------------------------------------------------
// ArgParser: flag and option values.
// -----------------------------------------------------------------------------


// Retrieve an Option instance by name.
static Option* ap_get_opt(ArgParser *parser, const char *name) {
    Option *opt = map_get(parser->options, name);
    if (opt == NULL) {
        fprintf(stderr, "Error: '%s' is not a registered flag or option name.\n", name);
        exit(1);
    }
    return opt;
}


// Returns the number of times the specified flag or option was found.
int ap_count(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return opt->count;
}


// Returns true if the specified flag or option was found.
bool ap_found(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return opt->count > 0;
}


// Returns the value of the specified string option.
char* ap_str_value(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return option_get_str(opt);
}


// Returns the value of the specified integer option.
int ap_int_value(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return option_get_int(opt);
}


// Returns the value of the specified floating-point option.
double ap_dbl_value(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return option_get_double(opt);
}


// Returns an option's values as a freshly-allocated array of string pointers.
// The array's memory is not affected by calls to ap_free().
char** ap_str_values(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return option_get_str_list(opt);
}


// Returns an option's values as a freshly-allocated array of integers. The
// array's memory is not affected by calls to ap_free().
int* ap_int_values(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return option_get_int_list(opt);
}


// Returns an option's values as a freshly-allocated array of doubles. The
// array's memory is not affected by calls to ap_free().
double* ap_dbl_values(ArgParser *parser, const char *name) {
    Option *opt = ap_get_opt(parser, name);
    return option_get_double_list(opt);
}


// -----------------------------------------------------------------------------
// ArgParser: positional arguments.
// -----------------------------------------------------------------------------


// Returns true if the parser has found one or more positional arguments.
bool ap_has_args(ArgParser *parser) {
    return parser->arguments->len > 0;
}


// Returns the number of positional arguments.
int ap_count_args(ArgParser *parser) {
    return parser->arguments->len;
}


// Returns the positional argument at the specified index.
char* ap_arg(ArgParser *parser, int index) {
    return parser->arguments->args[index];
}


// Returns the positional arguments as a freshly-allocated array of string
// pointers. The memory occupied by the returned array is not affected by
// calls to ap_free().
char** ap_args(ArgParser *parser) {
    char **args = malloc(sizeof(char*) * parser->arguments->len);
    memcpy(
        args,
        parser->arguments->args,
        sizeof(char*) * parser->arguments->len
    );
    return args;
}


// Attempts to parse and return the positional arguments as a freshly
// allocated array of integers. Exits with an error message on failure. The
// memory occupied by the returned array is not affected by calls to
// ap_free().
int* ap_args_as_ints(ArgParser *parser) {
    int *args = malloc(sizeof(int) * parser->arguments->len);
    for (int i = 0; i < parser->arguments->len; i++) {
        *(args + i) = try_str_to_int(parser->arguments->args[i]);
    }
    return args;
}


// Attempts to parse and return the positional arguments as a freshly
// allocated array of doubles. Exits with an error message on failure. The
// memory occupied by the returned array is not affected by calls to
// ap_free().
double* ap_args_as_doubles(ArgParser *parser) {
    double *args = malloc(sizeof(double) * parser->arguments->len);
    for (int i = 0; i < parser->arguments->len; i++) {
        *(args + i) = try_str_to_double(parser->arguments->args[i]);
    }
    return args;
}


// -----------------------------------------------------------------------------
// ArgParser: commands.
// -----------------------------------------------------------------------------


// Register a new command.
ArgParser* ap_cmd(ArgParser *parser, char *name) {
    parser->cmd_help = true;
    ArgParser *cmd_parser = ap_new();
    map_add_splitkey(parser->commands, name, cmd_parser);
    return cmd_parser;
}


// Register a callback function for a command.
void ap_callback(ArgParser *parser, void (*callback)(char*, ArgParser*)) {
    parser->callback = callback;
}


// Returns true if the parser has found a command.
bool ap_has_cmd(ArgParser *parser) {
    return parser->cmd_name != NULL;
}


// Returns the command name, if the parser has found a command.
char* ap_cmd_name(ArgParser *parser) {
    return parser->cmd_name;
}


// Returns the command's parser instance, if the parser has found a command.
ArgParser* ap_cmd_parser(ArgParser *parser) {
    return parser->cmd_parser;
}


// Toggles support for the automatic 'help' command.
void ap_cmd_help(ArgParser *parser, bool enable) {
    parser->cmd_help = enable;
}


// -----------------------------------------------------------------------------
// ArgParser: parse arguments.
// -----------------------------------------------------------------------------


// Parse an option of the form --name=value or -n=value.
static void ap_handle_equals_opt(ArgParser *parser, char *prefix, char *arg) {
    char *name = str_dup(arg);
    *strchr(name, '=') = '\0';
    char *value = strchr(arg, '=') + 1;

    Option *opt = map_get(parser->options, name);
    if (opt == NULL || opt->type == FLAG) {
        err(str("%s%s is not a recognised option name", prefix, name));
    }

    if (strlen(value) == 0) {
        err(str("missing value for %s%s", prefix, name));
    }

    option_try_set(opt, value);
    free(name);
}


// Parse a long-form option, i.e. an option beginning with a double dash.
static void ap_handle_long_opt(ArgParser *parser, char *arg, ArgStream *stream) {
    if (map_contains(parser->options, arg)) {
        Option *opt = map_get(parser->options, arg);
        if (opt->type == FLAG) {
            opt->count++;
        } else if (argstream_has_next(stream)) {
            option_try_set(opt, argstream_next(stream));
        } else {
            err(str("missing argument for --%s", arg));
        }
    } else if (strcmp(arg, "help") == 0 && parser->helptext != NULL) {
        puts(parser->helptext);
        exit(0);
    } else if (strcmp(arg, "version") == 0 && parser->version != NULL) {
        puts(parser->version);
        exit(0);
    } else {
        err(str("--%s is not a recognised flag or option name", arg));
    }
}


// Parse a short-form option, i.e. an option beginning with a single dash.
static void ap_handle_short_opt(ArgParser *parser, char *arg, ArgStream *stream) {
    for (size_t i = 0; i < strlen(arg); i++) {
        char keystr[] = {arg[i], 0};
        Option *opt = map_get(parser->options, keystr);
        if (opt == NULL) {
            if (arg[i] == 'h' && parser->helptext != NULL) {
                puts(parser->helptext);
                exit(0);
            } else if (arg[i] == 'v' && parser->version != NULL) {
                puts(parser->version);
                exit(0);
            } else if (strlen(arg) > 1) {
                err(str("'%c' in -%s is not a recognised flag or option name", arg[i], arg));
            } else {
                err(str("-%s is not a recognised flag or option name", arg));
            }
        } else if (opt->type == FLAG) {
            opt->count++;
        } else if (argstream_has_next(stream)) {
            option_try_set(opt, argstream_next(stream));
        } else if (strlen(arg) > 1) {
            err(str("missing argument for '%c' in -%s", arg[i], arg));
        } else {
            err(str("missing argument for -%s", arg));
        }
    }
}


// Parse a stream of string arguments.
static void ap_parse_stream(ArgParser *parser, ArgStream *stream) {
    bool is_first_arg = true;
    while (argstream_has_next(stream)) {
        char *arg = argstream_next(stream);

        // If we encounter a '--' argument, turn off option-parsing.
        if (strcmp(arg, "--") == 0) {
            while (argstream_has_next(stream)) {
                arglist_append(parser->arguments, argstream_next(stream));
            }
        }

        // Is the argument a long-form option or flag?
        else if (strncmp(arg, "--", 2) == 0) {
            if (strstr(arg, "=") != NULL) {
                ap_handle_equals_opt(parser, "--", arg + 2);
            } else {
                ap_handle_long_opt(parser, arg + 2, stream);
            }
        }

        // Is the argument a short-form option or flag?
        else if (arg[0] == '-') {
            if (strlen(arg) == 1 || isdigit(arg[1])) {
                arglist_append(parser->arguments, arg);
            } else if (strstr(arg, "=") != NULL) {
                ap_handle_equals_opt(parser, "-", arg + 1);
            } else {
                ap_handle_short_opt(parser, arg + 1, stream);
            }
        }

        // Is the argument a registered command?
        else if (is_first_arg && map_contains(parser->commands, arg)) {
            ArgParser *cmd_parser = map_get(parser->commands, arg);
            parser->cmd_name = arg;
            parser->cmd_parser = cmd_parser;
            ap_parse_stream(cmd_parser, stream);
            if (cmd_parser->callback != NULL) {
                cmd_parser->callback(arg, cmd_parser);
            }
        }

        // Is the argument the automatic 'help' command?
        else if (is_first_arg && parser->cmd_help && strcmp(arg, "help") == 0) {
            if (argstream_has_next(stream)) {
                char *name = argstream_next(stream);
                if (map_contains(parser->commands, name)) {
                    ArgParser *cmd_parser = map_get(parser->commands, name);
                    if (cmd_parser->helptext != NULL) {
                        puts(cmd_parser->helptext);
                    }
                    exit(0);
                } else {
                    err(str("'%s' is not a recognised command", name));
                }
            } else {
                err(str("the help command requires an argument"));
            }
        }

        // Otherwise add the argument to our list of positionals.
        else {
            arglist_append(parser->arguments, arg);
        }
        is_first_arg = false;
    }
}


// Parse an array of string arguments.
void ap_parse_array(ArgParser *parser, int len, char* args[]) {
    ArgStream *stream = argstream_new(len, args);
    ap_parse_stream(parser, stream);
    argstream_free(stream);
}


// Parse the application's command line arguments.
void ap_parse(ArgParser *parser, int argc, char* argv[]) {
    ap_parse_array(parser, argc - 1, argv + 1);
}


// -----------------------------------------------------------------------------
// ArgParser: utilities.
// -----------------------------------------------------------------------------


// Print a parser instance to stdout.
void ap_print(ArgParser *parser) {
    puts("Flags/Options:");
    if (parser->options->len > 0) {
        for (int i = 0; i < parser->options->len; i++) {
            char *name = map_key_at_index(parser->options, i);
            Option *opt = map_value_at_index(parser->options, i);
            char *optstr = option_to_str(opt);
            printf("  %s: %s\n", name, optstr);
            free(optstr);
        }
    } else {
        puts("  [none]");
    }
    puts("");

    arglist_print(parser->arguments);

    puts("\nCommand:");
    if (ap_has_cmd(parser)) {
        printf("  %s\n", ap_cmd_name(parser));
    } else {
        puts("  [none]");
    }
}
