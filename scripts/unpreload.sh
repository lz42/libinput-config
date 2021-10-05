#!/bin/sh

LIB="${1}/${2}.so"
PRELOAD="${DESTDIR}/etc/ld.so.preload"

if test -f ${PRELOAD}; then
	sed -i "\|${LIB}|d" ${PRELOAD}
fi
