#!/bin/sh
set -eu

cd "$(dirname "$0")"

# --- Arguments ---------------------------------------------------------------
mode="debug"
compiler="clang"
action="build"

while [ "$#" -gt 0 ]; do
    case "$1" in
        debug|release) mode="$1" ;;
        clang|gcc|cc) compiler="$1" ;;
        build|run) action="$1" ;;
        --)
            shift
            break
            ;;
        *)
            echo "usage: $0 [debug|release] [clang|gcc|cc] [build|run] [-- args...]" >&2
            exit 2
            ;;
    esac
    shift
done

if [ "$action" != "run" ] && [ "$#" -gt 0 ]; then
    echo "error: program arguments require the 'run' action" >&2
    exit 2
fi

# --- Flags -------------------------------------------------------------------
cc_cflags_gcc=""
cc_cflags_clang="-fdiagnostics-absolute-paths"
cc_common="-std=c23 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wundef -Wno-unused-function"
cc_debug="-O0 -g $cc_common"
cc_release="-O2 -g $cc_common"

# --- Choose Flags ------------------------------------------------------------
if   [ "$compiler" = "gcc" ];   then echo "[gcc compile]" >&2;   cc_compiler="${CC:-gcc} $cc_cflags_gcc"
elif [ "$compiler" = "clang" ]; then echo "[clang compile]" >&2; cc_compiler="${CC:-clang} $cc_cflags_clang"
elif [ "$compiler" = "cc" ];    then echo "[cc compile]" >&2;    cc_compiler="${CC:-cc}"
fi
if   [ "$mode" = "debug" ];   then echo "[debug mode]" >&2;   compile="$cc_compiler $cc_debug"
elif [ "$mode" = "release" ]; then echo "[release mode]" >&2; compile="$cc_compiler $cc_release"
fi

# --- Build -------------------------------------------------------------------
mkdir -p build
$compile fib.c -o build/fib

# --- Run ---------------------------------------------------------------------
if [ "$action" = "run" ]; then
    exec ./build/fib "$@"
fi
