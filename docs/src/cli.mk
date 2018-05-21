---
title: Command Line Interface
---

#### Options

Janus supports four kinds of options: boolean, string, integer, and floating-point.

* Boolean options take no arguments but are either present (*true*) or absent (*false*).

* String, integer, and floating-point options require arguments of the appropriate type.

An option can have an unlimited number of long-form aliases and single-character shortcuts: `--option`, `-o`.

Option values can be separated by either a space, `--opt 123`, or an equals symbol, `--opt=123`. Either syntax can be used with shortcuts: `-o 123`, `-o=123`.

Multiple shortcuts can be condensed into a single block, e.g. `-abc foo bar`. Trailing arguments are consumed in sequence as required by the options.

Options are registered with fallback values which are used if the option is not found. If an option is found multiple times its value is the final value encountered --- i.e. the value of `-o 123 -o 456` is `456`.



### Multivalued Options

Options can be treated as singular or multivalued as circumstances require. Each option maintains an internal array to which newly parsed values are appended; the (singular) value of the option is the final value in the array, or the fallback value if the array is empty.

For example, in the command below:

    $ myapp --foo 123 --foo 456

the value of the option `foo` is `456` but the array `[123, 456]` is also available for use if required.




### Positional Arguments

Options can be preceded, followed, or interspaced with positional arguments. Janus assembles all positional arguments together into a single, undifferentiated array of strings. Convenience functions are supplied for parsing these strings as integers or doubles.

Janus supports the standard `--` switch for turning off option-parsing. All arguments following a `--` will be treated as positional arguments, even if they begin with a single or double dash.



### Commands

Janus supports git-style command interfaces with arbitrarily-nested commands. Commands have builtin support for an automatic `--help` flag and an automatic `help <cmd>` command, i.e. the commands

    $ myapp <cmd> --help

and

    $ myapp help <cmd>

are functionally identical and will both print the help text registered with the command.
