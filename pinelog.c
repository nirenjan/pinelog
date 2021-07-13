/*
 * Pinelog lightweight logging library
 *
 * Copyright (C) 2021 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: MIT
 */

#include "config.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>

#include "pinelog.h"

/**********************************************************************
 * Configure defaults
 *********************************************************************/
#ifndef PINELOG_DEFAULT_STREAM
#define PINELOG_DEFAULT_STREAM stdout
#endif

#ifndef PINELOG_DEFAULT_LEVEL
#define PINELOG_DEFAULT_LEVEL LOG_LVL_ERROR
#endif

/**********************************************************************
 * Configure logging parameters
 *********************************************************************/
#ifndef PINELOG_SHOW_DATE
#define PINELOG_SHOW_DATE 0
#endif

#ifndef PINELOG_SHOW_LEVEL
#define PINELOG_SHOW_LEVEL 0
#endif

#ifndef PINELOG_SHOW_BACKTRACE
#define PINELOG_SHOW_BACKTRACE 0
#endif

/**********************************************************************
 * Configure level strings
 *********************************************************************/
#ifndef PINELOG_FATAL
#define PINELOG_FATAL "FATAL"
#endif

#ifndef PINELOG_ERROR
#define PINELOG_ERROR "ERROR"
#endif

#ifndef PINELOG_WARNING
#define PINELOG_WARNING "WARNING"
#endif

#ifndef PINELOG_INFO
#define PINELOG_INFO "INFO"
#endif

#ifndef PINELOG_DEBUG
#define PINELOG_DEBUG "DEBUG"
#endif

#ifndef PINELOG_TRACE
#define PINELOG_TRACE "TRACE"
#endif

/**********************************************************************
 * Global variables
 *********************************************************************/

/** Stream buffer */
static FILE *output_stream = NULL;

/** Default logging level */
static int log_level = PINELOG_DEFAULT_LEVEL;

/* Initialize defaults */
#if HAVE_FUNC_ATTRIBUTE_CONSTRUCTOR
__attribute__((constructor))
#endif
void log_set_defaults(void)
{
    output_stream = PINELOG_DEFAULT_STREAM;
    log_level = PINELOG_DEFAULT_LEVEL;
}

int log_set_output_stream(FILE *stream)
{
    if (stream == NULL) {
        return EINVAL;
    }

    /* If current output stream is not stdout or stderr, then close it */
    if (output_stream != stdout && output_stream != stderr) {
        fclose(output_stream);
    }

    setlinebuf(stream);
    output_stream = stream;
    return 0;
}

int log_set_output_file(const char *file)
{
    FILE *stream;
    if (file == NULL) {
        return EINVAL;
    }

    errno = 0;
    stream = fopen(file, "w");
    if (stream == NULL) {
        return errno;
    }

    return log_set_output_stream(stream);
}

int log_get_level(void)
{
    return log_level;
}

int log_set_level(int level)
{
    if (level < LOG_LVL_NONE || level > LOG_LVL_TRACE) {
        return EINVAL;
    }

    log_level = level;
    return 0;
}

/**********************************************************************
 * Log the message to the output stream
 *********************************************************************/
void log_message(int level, const char *file, int line, const char *fmt, ...)
{
    va_list ap;

    /* Don't log anything if the level is not severe enough */
    if (level > log_level || level < 0) {
        return;
    }

    /* Cap the log level */
    if (level > LOG_LVL_TRACE) {
        level = LOG_LVL_TRACE;
    }

    /* Validate and set output stream */
    if (output_stream == NULL) {
        output_stream = stdout;
    }

    #if PINELOG_SHOW_DATE
    do {
        time_t t;
        struct tm *tmp;
        char date_string[20];
        t = time(NULL);
        tmp = localtime(&t);
        strftime(date_string, sizeof(date_string), "%F %T ", tmp);
        fputs(date_string, out_stream);
    } while (0);
    #endif

    #if PINELOG_SHOW_LEVEL
    do {
        static const char *level_strings[] = {
            PINELOG_FATAL,
            PINELOG_ERROR,
            PINELOG_WARNING,
            PINELOG_INFO,
            PINELOG_DEBUG,
            PINELOG_TRACE,
        };

        fputs(level_strings[level], out_stream);
        fputs(": ", out_stream);
    } while (0)
    #endif

    #if PINELOG_SHOW_BACKTRACE
        fprintf(out_stream, "%s:%d ", file, line);
    #endif

    va_start(ap, fmt);
    vfprintf(output_stream, fmt, ap);
    va_end(ap);
}
