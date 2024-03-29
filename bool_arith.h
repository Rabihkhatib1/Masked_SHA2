#include <stdint.h>
#include <stdlib.h>

#define WORD_SIZE 64
#define MODULO (1 << WORD_SIZE) - 1

typedef struct {
    uint64_t xs;
    uint64_t r;
} share;

