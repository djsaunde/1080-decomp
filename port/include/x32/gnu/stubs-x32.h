/* PORT_64 (plan 21): x32 compiles parse the x86-64 glibc headers; no x32 libc exists or is linked.
 * glibc's gnu/stubs.h includes this file for __ILP32__; nothing is stubbed. */
