#ifndef BOOL_ARITH_INTERLEAVING_H
#define BOOL_ARITH_INTERLEAVING_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bool_arith.h"

#define WORD_SIZE 64
#if WORD_SIZE == 32
typedef uint32_t uint_word_t;
#elif WORD_SIZE == 64
typedef uint64_t uint_word_t;
typedef uint32_t uint_half_t;
#else
#error "Unsupported word size"
#endif
#define MODULO ((uint_word_t)(-1))

// typedef struct {
//     uint_word_t xs;
//     uint_word_t xr;
// } share;

typedef struct {
    uint_half_t x0;
    uint_half_t x1;
    uint_half_t x2;
    uint_half_t x3;
} share_intlv;

uint32_t rand_count;

void randombytes_intlv(uint_word_t *ptr, int size);

void bool_share_intlv(share s, uint_word_t x);
share bool_share_r_intlv(share s, uint_word_t x);
uint_word_t bool_comb_intlv(share x);

void arith_share_intlv(share s, uint_word_t x) ;
share arith_share_r_intlv(share s, uint_word_t x);
uint_word_t arith_comb_intlv(share x);

void b2a_intlv(share_intlv *x);
void a2b_intlv(share *x);

share andm_intlv(share x,share y);
void andmn_intlv(share *z,share x,share y);
share xor_share_intlv(share x, share y);
share SecAddGoubin_intlv(share x, share y);

FILE *frandom;

#endif