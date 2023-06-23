#!/bin/sh
set -xe
clang -Wall -Wextra -Werror -Wpedantic -lwallycore -lsodium -o seed_gen seed_gen.c
