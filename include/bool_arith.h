#ifndef BOOL_ARITH_H
#define BOOL_ARITH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD_SIZE 64
#if WORD_SIZE == 32
typedef uint32_t uint_word_t;
#elif WORD_SIZE == 64
typedef uint64_t uint_word_t;
#else
#error "Unsupported word size"
#endif
#define MODULO ((uint_word_t)(-1))

typedef struct {
    uint_word_t xs;
    uint_word_t xr;
} share;

uint32_t rand_count;

void randombytes(uint_word_t *ptr, int size);

void bool_share(share s, uint_word_t x);
share bool_share_r(share s, uint_word_t x);
uint_word_t bool_comb(share x);

void arith_share(share s, uint_word_t x) ;
share arith_share_r(share s, uint_word_t x);
uint_word_t arith_comb(share x);

void b2a(share *x);
void a2b(share *x);

share andm(share x,share y);
void andmn(share *z,share x,share y);
share xor_share(share x, share y);
share SecAddGoubin(share x, share y);

FILE *frandom;

#endif