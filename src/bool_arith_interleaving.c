#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bool_arith_interleaving.h"
#include "bool_arith.h"
#include "sha512_masked.h"

#ifndef GEN
void randombytes_intlv(uint_word_t *ptr, int size) {
    FILE *frandom = fopen("/dev/urandom", "r");
    fread((ptr), (size), 1, frandom);
    fclose(frandom);
    rand_count += 1;
}
#else

void randombytes_intlv(uint_word_t *ptr, int size) {
    fread((ptr), (size), 1, frandom);
    // printf("rand %x = %lx\n", rand_count, *ptr);
    rand_count += 1;
}
#endif

void bool_share_intlv(share s, uint_word_t x) {
    uint_word_t rb = 0;
    randombytes_intlv(&rb,8);
    uint_word_t xb = x ^ rb;
    s.xs = xb;
    s.xr  = rb;
}

share bool_share_r_intlv(share s, uint_word_t x) {
    uint_word_t rb = 0;
    randombytes_intlv(&rb,8);
    uint_word_t xb = x ^ rb;
    s.xs = xb;
    s.xr  = rb;
    return s;
}


uint_word_t bool_comb_intlv(share x){
    return (x.xr ^ x.xs);
}

void arith_share_intlv(share s, uint_word_t x){
    uint_word_t ra = 0;
    randombytes_intlv(&ra,8);
    uint_word_t xa = x - ra;
    s.xs = xa;
    s.xr  = ra;    
}

share arith_share_r_intlv(share s, uint_word_t x){
    uint_word_t ra = 0;
    randombytes_intlv(&ra,8);
    uint_word_t xa = x - ra;
    s.xs = xa;
    s.xr  = ra;    
    return s;
}

uint_word_t arith_comb_intlv(share x){
    return (x.xr + x.xs) & MODULO;
}

void b2a_intlv(share_intlv *x) {
    uint_word_t y = 0x9234567812312345;
    // randombytes(&y, 8);

    // Extract and manipulate the interleaved bits directly
    uint_word_t T = ((x->x0 & 0xFFFF) ^ (y & 0xFFFF)) |
                    (((x->x1 & 0xFFFF) ^ ((y >> 16) & 0xFFFF)) << 16) |
                    (((x->x2 & 0xFFFF) ^ ((y >> 32) & 0xFFFF)) << 32) |
                    (((x->x3 & 0xFFFF) ^ ((y >> 48) & 0xFFFF)) << 48);

    T = (T - y) & MODULO;
    printf("b2a_intlv T = %lx",T);
    T = ((T & 0xFFFF) ^ (x->x0 & 0xFFFF)) |
        (((T >> 16 & 0xFFFF) ^ (x->x1 & 0xFFFF)) << 16) |
        (((T >> 32 & 0xFFFF) ^ (x->x2 & 0xFFFF)) << 32) |
        (((T >> 48 & 0xFFFF) ^ (x->x3 & 0xFFFF)) << 48);

    y = ((y & 0xFFFF) ^ (x->x0 >> 16)) |
        (((y >> 16 & 0xFFFF) ^ (x->x1 >> 16)) << 16) |
        (((y >> 32 & 0xFFFF) ^ (x->x2 >> 16)) << 32) |
        (((y >> 48 & 0xFFFF) ^ (x->x3 >> 16)) << 48);

    uint_word_t xa = ((x->x0 & 0xFFFF) ^ (y & 0xFFFF)) |
                     (((x->x1 & 0xFFFF) ^ ((y >> 16) & 0xFFFF)) << 16) |
                     (((x->x2 & 0xFFFF) ^ ((y >> 32) & 0xFFFF)) << 32) |
                     (((x->x3 & 0xFFFF) ^ ((y >> 48) & 0xFFFF)) << 48);
    xa = (xa - y) & MODULO;
    xa = ((xa & 0xFFFF) ^ (T & 0xFFFF)) |
         (((xa >> 16 & 0xFFFF) ^ (T >> 16 & 0xFFFF)) << 16) |
         (((xa >> 32 & 0xFFFF) ^ (T >> 32 & 0xFFFF)) << 32) |
         (((xa >> 48 & 0xFFFF) ^ (T >> 48 & 0xFFFF)) << 48);

    // Update x0, x1, x2, and x3 with the new values
    x->x0 = (uint_half_t)((xa & 0xFFFF) | ((x->x0 & 0xFFFF0000)));
    x->x1 = (uint_half_t)(((xa >> 16) & 0xFFFF) | ((x->x1 & 0xFFFF0000)));
    x->x2 = (uint_half_t)(((xa >> 32) & 0xFFFF) | ((x->x2 & 0xFFFF0000)));
    x->x3 = (uint_half_t)(((xa >> 48) & 0xFFFF) | ((x->x3 & 0xFFFF0000)));
}

void a2b_intlv(share *x) {
    uint_word_t y = 0;
    randombytes_intlv(&y,8);
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

// uint_word_t andm(share x,share y) {
//     uint_word_t xs = x.xs, xr = x.xr, ys = y.xs, yr = y.xr;
//     return (~ys & (~yr & xr | yr & xs) | ys & (yr & xr | ~yr & xs) & MODULO);
// }

share andm_intlv(share x, share y) {
    share r,z;

    // Initialize random seed
    randombytes_intlv(&r.xs,8);

    // Unrolling the loops
    // r.xs &= rand() & 1;
    r.xr = (r.xs ^ (x.xs & y.xr)) ^ (x.xr & y.xs);

    z.xs = x.xs & y.xs;
    z.xs = z.xs ^ r.xs;

    z.xr = x.xr & y.xr;
    z.xr = z.xr ^ r.xr;

    return z;
}

void andmn_intlv(share *z,share x,share y) {
    uint_word_t xs = x.xs, xr = x.xr, ys = y.xs, yr = y.xr;
    z->xs = ~ys & (~yr & ~xr | yr & ~xs) | ys & (yr & ~xr | ~yr & ~xs) & MODULO;
}

share xor_share_intlv(share x, share y) {
    share z;
    z.xs = x.xs ^ y.xs;
    z.xr = x.xr ^ y.xr;
    return z;
}

share SecAddGoubin_intlv(share x, share y) {
    share w, a, z, ua;
    share u = {0};

    // Compute w
    w = andm_intlv(x, y);

    // Compute a
    a = xor_share_intlv(x,y);

    // Perform the loop
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 64; ++j) {
            ua = andm_intlv(u, a);
            u  = xor_share_intlv(ua, w); 
            u.xs = u.xs * 2;
            u.xr = u.xr * 2;
        }
    }

    // Compute z
    z = xor_share_intlv(x,y);
    z = xor_share_intlv(z,u);

    return z;
}