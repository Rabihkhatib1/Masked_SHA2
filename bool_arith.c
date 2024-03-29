#include <stdint.h>
#include <stdlib.h>
#include <bool_arith.h>

uint64_t urandom_to_int() {
    // You should implement this function to generate a random 64-bit unsigned integer
    return rand();
}

share bool_split(uint64_t x) {
    r = urandom_to_int(size_bytes(x))
    xb = x ^ r
    res = [xb,r]
    return res
}

uint64_t bool_comb(share x){
    return x[0] ^ x[1]
}

share arith_split(uint64_t x){
    r = urandom_to_int(size_bytes(x))
    xa = x - r
    res = [xa,r]
    return res
}

uint64_t arith_comb(x){
    return (x[0] + x[1]) % 2**64
}

void b2a(share x) {
    uint64_t y = urandom_to_int();
    uint64_t T = x.xs ^ y;
    T = (T - y) & MODULO;
    T = T ^ x.xs;
    y = y ^ x.r;
    uint64_t xa = x.xs ^ y;
    xa = (xa - y) & MODULO;
    xa = xa ^ T;
    x.xs = xa;
}

void a2b(share x) {
    uint64_t y = urandom_to_int();
    uint64_t T = (2 * y) & MODULO;
    uint64_t xb = y ^ x.r;
    uint64_t O = y & xb;
    xb = T ^ x.xs;
    y = y ^ xb;
    y = y & x.r;
    O = O ^ y;
    y = T & x.xs;
    O = O ^ y;
    for (uint64_t i = 1; i < WORD_SIZE; i++) {
        y = T & x.r;
        y = y ^ O;
        T = T & x.xs;
        y = y ^ T;
        T = (2 * y) & MODULO;
    }
    xb = xb ^ T;
    x.xs = xb;
}