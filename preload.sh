#!/bin/sh

LIB="${1}/${2}.so"
PRELOAD='/etc/ld.so.preload'

if test -z $(grep ${LIB} ${PRELOAD} 2> /dev/null); then
	echo ${LIB} >> ${PRELOAD}
fi
