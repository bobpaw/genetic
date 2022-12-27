# A Genetic Algorithm [![Build Status](https://travis-ci.org/bobpaw/genetic.svg?branch=master)](https://travis-ci.org/bobpaw/genetic)
Implemented in C (and soon C++).
Has an ncurses program to view generations.

# Prerequisites
 - A C++ compiler.
 - cmake (3.11 or later)
 - curses (ncurses or PDCurses are recommended)
 - pkg-config (recommended)

# Installation
1. Install prerequisites.
2. `git clone http://github.com/bobpaw/genetic.git`
3. `cd genetic`
2. `mkdir build`
3. `cd build`
3. `cmake ..`
4. `cmake --build .`

The `genetic` executable is now in `build/src/genetic`.

To run tests: `make test`.

# News
Refer to the [NEWS.md](NEWS.md) file.

# ChangeLog
Refer to the Git log for a complete revision history.

# Copyright
Copyright (c) 2020-2022 Aiden Woodruff.

This work is governed by the MIT License. See [COPYING](COPYING) for the full
license text.
