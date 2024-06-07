#ifndef SHA512_H
#define SHA512_H

#include <stddef.h>
#include <stdint.h>
#include "bool_arith.h"

/* state */
typedef struct sha512_context_m {
    uint64_t  length, state[8];
    share shares[8];
    size_t curlen;
    unsigned char bufr[128];
    unsigned char bufs[128];
} sha512_context_m;

#define GEN 1

int sha512_init_masked(sha512_context_m *md);
// int sha512_final_masked(sha512_context_m *md, unsigned char *out);
int sha512_update_masked(sha512_context_m *md, const unsigned char *in, size_t inlen);
int sha512_final_masked(sha512_context_m * md, unsigned char *out, const unsigned char *in_s, const unsigned char *in_r, size_t inlen);
// int sha512_masked(const unsigned char *message, size_t message_len, unsigned char *out);
int sha512_masked(const unsigned char *message_s, const unsigned char *message_r, size_t message_len, unsigned char *out); 

#endif