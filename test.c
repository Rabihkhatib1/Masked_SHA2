// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>

// #define RANDOM_DATA(ptr, size) \
// FILE *frandom = fopen("/dev/urandom", "r"); \
// fread((ptr), (size), 1, frandom); \
// fclose(frandom);

void randombytes(void *ptr, int size) {
    FILE *frandom = fopen("/dev/urandom", "r");
    fread((ptr), (size), 1, frandom);
    fclose(frandom);
}

int main() {
    // Write C code here
    uint8_t data[8] = {0};
    uint64_t data64 = 0;
    // RANDOM_DATA(data, 8);
    randombytes(&data64, 8);
    for(int i=0;i<8;i++) {
        printf("%x ",data[i]);
    }
    printf("\n");
    randombytes(&data64, 8);
    printf("%lx\n",data64);
    randombytes(&data64, 8);
    printf("%lx\n",data64);
    randombytes(&data64, 8);
    printf("%lx\n",data64);
    return 0;
}