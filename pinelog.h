/*
 * Pinelog lightweight logging library
 *
 * Copyright (C) 2021 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file logging.h
 * @brief Logging utility library
 *
 * This file contains the prototypes for the pinelog logging library
 * used by any programs that need to log messages.
 *
 * @author Nirenjan Krishnan (nirenjan@nirenjan.org)
 */
#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Logging levels
 *
 * The log levels indicate the lowest severity level that will actually be
 * logged to the logging framework.
 */
enum {
    /** No messages will be logged */
    LOG_LVL_NONE = -1,

    /** Only fatal messages will be logged */
    LOG_LVL_FATAL,

    /** Error messages. This is the default log level */
    LOG_LVL_ERROR,

    /** Warning messages */
    LOG_LVL_WARNING,

    /** Informational messages */
    LOG_LVL_INFO,

    /** Debug messages */
    LOG_LVL_DEBUG,

    /** Trace messages */
    LOG_LVL_TRACE,
};

/**
 * @brief Set the default log level and output stream
 */
void log_set_defaults(void);

/**
 * @brief Set the output stream. Must be a FILE pointer.
 *
 * @param[in]   stream  Pointer to the output stream
 *
 * @returns 0 on success, EINVAL if the pointer is not valid.
 */
int log_set_output_stream(FILE *stream);

/**
 * @brief Set the output file.
 *
 * @param[in]   file    Filename to write to
 *
 * @returns 0 on success, EINVAL if the filename pointer is not valid, other
 * error if the file could not be opened for writing.
 */
int log_set_output_file(const char *file);

/**
 * @brief Set the logging level
 *
 * @param[in]   level   Level to filter
 *
 * @returns 0 on success, EINVAL if the level is not valid
 */
int log_set_level(int level);

/**
 * @brief Get the logging level
 *
 * @returns the configured logging level
 */
int log_get_level(void);

/**
 * @brief Log a message to the logger
 *
 * This is the actual function that logs the message. The application should
 * never need to call this directly, but instead, should always use the
 * \code LOG_* macros.
 *
 * @param[in]   level   Level to log the message at
 * @param[in]   fmt     Format string
 *
 * @returns None
 */
void log_message(int level, const char *file, int line, const char *fmt, ...);

#define LOG_FATAL(fmt, ...) do { \
    if (LOG_LVL_FATAL <= log_get_level()) { \
        log_message(LOG_LVL_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } \
    exit(1); \
} while (0)

#define LOG_ERROR(fmt, ...) do { \
    if (LOG_LVL_ERROR <= log_get_level()) { \
        log_message(LOG_LVL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } \
} while (0)

#define LOG_WARN(fmt, ...) do { \
    if (LOG_LVL_WARNING <= log_get_level()) { \
        log_message(LOG_LVL_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_INFO(fmt, ...) do { \
    if (LOG_LVL_INFO <= log_get_level()) { \
        log_message(LOG_LVL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_DEBUG(fmt, ...) do { \
    if (LOG_LVL_DEBUG <= log_get_level()) { \
        log_message(LOG_LVL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } \
} while(0)

#define LOG_TRACE(fmt, ...) do { \
    if (LOG_LVL_TRACE <= log_get_level()) { \
        log_message(LOG_LVL_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
    } \
} while(0)

#ifdef __cplusplus
}
#endif

#endif // !defined LOGGING_H
