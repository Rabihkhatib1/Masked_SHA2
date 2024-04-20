#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bool_arith.h"

void randombytes(uint_word_t *ptr, int size) {
    FILE *frandom = fopen("/dev/urandom", "r");
    fread((ptr), (size), 1, frandom);
    fclose(frandom);
}

void bool_share(share s, uint_word_t x) {
    uint_word_t rb = 0;
    randombytes(&rb,8);
    uint_word_t xb = x ^ rb;
    s.xs = xb;
    s.xr  = rb;
}

share bool_share_r(share s, uint_word_t x) {
    uint_word_t rb = 0;
    randombytes(&rb,8);
    uint_word_t xb = x ^ rb;
    s.xs = xb;
    s.xr  = rb;
    return s;
}


uint_word_t bool_comb(share x){
    return (x.xr ^ x.xs);
}

void arith_share(share s, uint_word_t x){
    uint_word_t ra = 0;
    randombytes(&ra,8);
    uint_word_t xa = x - ra;
    s.xs = xa;
    s.xr  = ra;    
}

share arith_share_r(share s, uint_word_t x){
    uint_word_t ra = 0;
    randombytes(&ra,8);
    uint_word_t xa = x - ra;
    s.xs = xa;
    s.xr  = ra;    
    return s;
}

uint_word_t arith_comb(share x){
    return (x.xr + x.xs) & MODULO;
}

void b2a(share *x) {
    uint_word_t y = 0;
    randombytes(&y,8);
    uint_word_t T = x->xs ^ y;
    T = (T - y) & MODULO;
    T = T ^ x->xs;
    y = y ^ x->xr;
    uint_word_t xa = x->xs ^ y;
    xa = (xa - y) & MODULO;
    xa = xa ^ T;
    x->xs = xa;
}

void a2b(share *x) {
    uint_word_t y = 0;
    randombytes(&y,8);
    uint_word_t T = (2 * y) & MODULO;
    uint_word_t xb = y ^ x->xr;
    uint_word_t O = y & xb;
    xb = T ^ x->xs;
    y = y ^ xb;
    y = y & x->xr;
    O = O ^ y;
    y = T & x->xs;
    O = O ^ y;
    for (int i = 1; i < WORD_SIZE; i++) {
        y = T & x->xr;
        y = y ^ O;
        T = T & x->xs;
        y = y ^ T;
        T = (2 * y) & MODULO;
    }
    xb = xb ^ T;
    x->xs = xb;
}

uint_word_t andm(share x,share y) {
    uint_word_t xs = x.xs, xr = x.xr, ys = y.xs, yr = y.xr;
    return (~ys & (~yr & xr | yr & xs) | ys & (yr & xr | ~yr & xs) & MODULO);
}

void andmn(share *z,share x,share y) {
    uint_word_t xs = x.xs, xr = x.xr, ys = y.xs, yr = y.xr;
    z->xs = ~ys & (~yr & ~xr | yr & ~xs) | ys & (yr & ~xr | ~yr & ~xs) & MODULO;
}