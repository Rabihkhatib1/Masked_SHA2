#include <stdint.h>
#include <stdlib.h>

#define WORD_SIZE 64
#define MODULO (1 << WORD_SIZE) - 1

typedef struct {
    uint64_t xs;
    uint64_t xr;
} share;

void bool_share(share s, uint64_t x);
uint64_t bool_comb(share x);

void arith_share(share s, uint64_t x) ;
uint64_t arith_comb(share x);

void b2a(share x);
void a2b(share x);

uint64_t andm(share x,share y);
void andmn(share z,share x,share y);