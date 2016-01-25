#include <stdio.h>
#include <stdlib.h>
#define ts_exit_error(...) do {fprintf(stderr, __VA_ARGS__); exit(1);} while(0);