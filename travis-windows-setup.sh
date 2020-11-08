#!/usr/bin/env sh

set -e

if [ "${TRAVIS_OS_NAME}" = "windows" ]; then
	choco install make
	wget -q https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.1.tar.gz
	tar zxf ncurses-6.1.tar.gz
	cd ncurses-6.1
	sh -x configure --without-pkg-config && make -j2
fi
