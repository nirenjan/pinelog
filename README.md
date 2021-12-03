Pinelog - a lightweight logging API
===================================

Pinelog is a lightweight logging API for C programs that's designed to be
included in your program source code. Parameters for Pinelog are configured at
build time by means of preprocessor flags.

# Usage
## Logging macros

Pinelog uses `printf` style formatting, using the following list of macros. The
macro indicates the level at which the message is logged. Note that Pinelog will
automatically append a trailing newline to your log message, so you should not
include one in your format string.

* `PINELOG_FATAL`
* `PINELOG_ERROR`
* `PINELOG_WARN`
* `PINELOG_INFO`
* `PINELOG_DEBUG`
* `PINELOG_TRACE`

**Note:** `PINELOG_FATAL` is used when the program encounters a fatal condition
and needs to abort. This will log the fatal message and terminate the program
with an exit code of 1.

### Example

```C
PINELOG_INFO("configuration file %s not found, using defaults", config_file);
```

## Modules

Pinelog supports individual logging modules, which can be used by an application
to individually control the log level for each module separately. This is
managed at runtime, where the application must call `pinelog_init` to register a
total count of modules, and then, for each module, it must call
`pinelog_setup_module` to initialize the module name for that module. The module
name is printed in the output.

Pinelog also allows the log level of the module to default to the global log
level. By default, module log level is set to `PINELOG_LVL_NOTSET`, which will
force Pinelog to use the global log level for that module.

In order to use the given module, the application must define `PINELOG_MODULE`
before the line `#include "pinelog.h"`. If it is not defined, Pinelog will
default to using the root module.

### Example

#### Initialization

```C
pinelog_init(2); // Initialize 2 modules
pinelog_setup_module(0, "foo"); // Module 0 name is "foo"
pinelog_setup_module(1, "bar"); // Module 1 name is "bar"
```

#### Inside module

```C
#define PINELOG_MODULE MODULE_FOO
#include "pinelog.h"
```

## Logging levels

The default logging level is `ERROR`, and this can be controlled by the
preprocessor flag `PINELOG_DEFAULT_LEVEL`.

The program can control the level at which messages can be logged at runtime,
by using the `pinelog_set_level` function. This function takes in the level
definition, which is one of the following, in increasing order of priority.

* `PINELOG_LVL_TRACE`
* `PINELOG_LVL_DEBUG`
* `PINELOG_LVL_INFO`
* `PINELOG_LVL_WARNING`
* `PINELOG_LVL_ERROR`
* `PINELOG_LVL_FATAL`
* `PINELOG_LVL_NONE`
* `PINELOG_LVL_NOTSET` - for modules only

Setting the level to a given priority suppresses all log messages of lower
priority, i.e., if the level is set to `PINELOG_LVL_ERROR`, messages at
`WARNING` level and below will be suppressed, but `ERROR` and `FATAL` messages
will be logged.

**Note:** `PINELOG_LVL_NONE` suppresses all log messages, but `PINELOG_FATAL`
will still terminate the program, even though nothing is logged.

**Note:** `PINELOG_LVL_NOTSET` is only applicable to module log level, it will
be rejected if the application attempts to use it for the global log level.

### Example

```C
pinelog_set_level(PINELOG_LVL_WARNING);
pinelog_set_module_level(MODULE_ID, PINELOG_LVL_TRACE);
```

```
-DPINELOG_DEFAULT_LEVEL=PINELOG_LVL_WARNING
```

## Output redirection

Pinelog defaults to writing the log messages to standard output, and this can
be controlled by the preprocessor flag `PINELOG_DEFAULT_STREAM`.

However, the application can redirect log messages at runtime to a different
`FILE *` stream, or to a file by using one of the following two methods:

```C
FILE *out = fopen("/run/app.fifo", "w");
pinelog_set_output_stream(out);
pinelog_set_output_file("/var/log/app.log");
```

```
-DPINELOG_DEFAULT_STREAM=stderr
```

## Logging format

Pinelog uses an opinionated logging format that is fixed as follows. Fields
within `[]` are optional and controlled by build time flags.

    [2021-07-14 11:08:04 ][ERROR: ][./test_pinelog.c:108 ][module-name: ]formatted message.

The program can be controlled by the following preprocessor flags, all of which
default to `0` (disabled). Set the flag to `1` to enable it.

* `PINELOG_SHOW_DATE` - Display the ISO 8601 date and time when the message is
  logged.
* `PINELOG_SHOW_LEVEL` - Display the level at which the message is logged.
* `PINELOG_SHOW_BACKTRACE` - Display the file and line where the message is
  logged.

Module name will always be displayed, if it is not the root module.

Set these flags by using the `-D` compiler argument, .e.g.
`-DPINELOG_SHOW_LEVEL=1 -DPINELOG_SHOW_DATE=1`

### Level strings

The application can control the level strings displayed by means of preprocessor
flags, if the application wishes to display the log messages in a language other
than English. This can be achieved by means of the following preprocessor
definitions.

* `PINELOG_FATAL_STR`
* `PINELOG_ERROR_STR`
* `PINELOG_WARNING_STR`
* `PINELOG_INFO_STR`
* `PINELOG_DEBUG_STR`
* `PINELOG_TRACE_STR`

### Example

```
-DPINELOG_ERROR_STR=\"E\" -DPINELOG_FATAL_STR=\"F\"
```

## Temporary buffer

By default, Pinelog will write the individual components of the log output,
i.e., date, log level, backtrace, module and log message individually to the
output stream. The drawback of this is that if Pinelog is used in a
multithreaded application, then the messages from multiple threads may be
interleaved. In order to avoid this, you can define `PINELOG_BUFFER_SZ` to a
positive value. This should be of sufficient size such that the largest log
message can fit into this buffer, along with the individual components
(timestamp, log level, etc.). A reasonable starting point for this is 256 bytes.

Note that this will result in a greater use of stack space, so applications with
limited stack space should not use this, or they risk a stack overflow.

### Usage

Add the following definition to your CFLAGS when building Pinelog

```
-DPINELOG_BUFFER_SZ=256
```

# Integrating Pinelog

Pinelog is intended to be integrated into your application source tree, either
by means of including the sources directly, or by including the repository as
a Git submodule or subtree.
