#include <stdint.h>
#include <stdlib.h>
#include <bool_arith.h>

uint64_t urandom_to_int() {
    // You should implement this function to generate a random 64-bit unsigned integer
    return rand();
}

void bool_share(share s, uint64_t x) {
    uint64_t rb = urandom_to_int(size_bytes(x));
    uint64_t xb = x ^ rb;
    s.xs = xb;
    s.xr  = rb;
    // share res = {.xs = xb, .r = rb};
    // return res;
}

uint64_t bool_comb(share x){
    return (x.xr ^ x.xs);
}

void arith_share(share s, uint64_t x){
    uint64_t ra = urandom_to_int(size_bytes(x));
    uint64_t xa = x - ra;
    s.xs = xa;
    s.xr  = ra;    
    // share res = {.xs = xa, .r = ra};
    // return res;
}

uint64_t arith_comb(share x){
    return (x.xr + x.xs) & MODULO;
}

void b2a(share x) {
    uint64_t y = urandom_to_int();
    uint64_t T = x.xs ^ y;
    T = (T - y) & MODULO;
    T = T ^ x.xs;
    y = y ^ x.xr;
    uint64_t xa = x.xs ^ y;
    xa = (xa - y) & MODULO;
    xa = xa ^ T;
    x.xs = xa;
}

void a2b(share x) {
    uint64_t y = urandom_to_int();
    uint64_t T = (2 * y) & MODULO;
    uint64_t xb = y ^ x.xr;
    uint64_t O = y & xb;
    xb = T ^ x.xs;
    y = y ^ xb;
    y = y & x.xr;
    O = O ^ y;
    y = T & x.xs;
    O = O ^ y;
    for (uint64_t i = 1; i < WORD_SIZE; i++) {
        y = T & x.xr;
        y = y ^ O;
        T = T & x.xs;
        y = y ^ T;
        T = (2 * y) & MODULO;
    }
    xb = xb ^ T;
    x.xs = xb;
}

uint64_t andm(share x,share y) {
    uint64_t xs = x.xs, xr = x.xr, ys = y.xs, yr = y.xr;
    return (~ys & (~yr & xr | yr & xs) | ys & (yr & xr | ~yr & xs) & MODULO);
}

void andmn(share z,share x,share y) {
    uint64_t xs = x.xs, xr = x.xr, ys = y.xs, yr = y.xr;
    z.xs = ~ys & (~yr & ~xr | yr & ~xs) | ys & (yr & ~xr | ~yr & ~xs) & MODULO;
}