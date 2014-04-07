#!/bin/sh -e
 
test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.
autoreconf --force --install --verbose -I config -I m4 "$srcdir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"
