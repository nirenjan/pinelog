# Top level Automake for pinelog
#
# Copyright (C) 2012-2018 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: MIT

ACLOCAL_AMFLAGS = -I m4

# Extra files that need to be in the distribution
EXTRA_DIST = \
	LICENSE \
	README.md

noinst_LTLIBRARIES = libpinelog.la

# pinelog logging library
libpinelog_la_SOURCES = pinelog.c
libpinelog_la_CFLAGS = @PINELOG_CFLAGS@ $(WARN_CFLAGS)
libpinelog_la_LDFLAGS = $(WARN_LDFLAGS)

