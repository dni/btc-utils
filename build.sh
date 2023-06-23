#!/bin/sh
set -xe
clang -Wall -Wextra -Wpedantic -lwallycore -lsodium -o btc_utils btc_utils.c
