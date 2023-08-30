#!/bin/sh
set -xe
LD_LIBRARY_PATH=/usr/local/lib clang -Wall -Wextra -Wpedantic -lwallycore -lsodium -o btc_utils btc_utils.c
