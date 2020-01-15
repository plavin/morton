// Author: Patrick Lavin
// This code prints a 2d or 3d morton ordering

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define VERBOSE 0

// Taken from https://stackoverflow.com/a/30562230
uint64_t even_bits(uint64_t x)
{
    x = x & 0x5555555555555555;
    x = (x | (x >> 1))  & 0x3333333333333333;
    x = (x | (x >> 2))  & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4))  & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8))  & 0x0000FFFF0000FFFF;
    x = (x | (x >> 16)) & 0x00000000FFFFFFFF;
    return (uint64_t)x;
}

void unpack_2d(uint64_t d, uint64_t *x, uint64_t *y)
{
    *x = even_bits(d);
    *y = even_bits(d >> 1);
}

void z_order_2d(uint64_t dim, uint32_t *list)
{
    uint64_t i, x, y, idx = 0, extra = 0;

    for (i = 0; i < dim * dim + extra; i++) {
        unpack_2d(i, &y, &x);
        if (x >= dim || y >= dim) {
            extra++;
            continue;
        }
        list[idx++] = (uint32_t)(x*dim + y);

        #if VERBOSE
            printf("(%lu,%lu) -> %lu\n", x, y, ((uint64_t)x)*dim + y);
        #endif

    }
}


// Taken from https://stackoverflow.com/a/28358035
uint64_t third_bits(uint64_t x) {
    x = x & 0x9249249249249249;
    x = (x | (x >> 2))  & 0x30C30C30C30C30C3;
    x = (x | (x >> 4))  & 0xF00F00F00F00F00F;
    x = (x | (x >> 8))  & 0x00FF0000FF0000FF;
    x = (x | (x >> 16)) & 0xFFFF00000000FFFF;
    x = (x | (x >> 32)) & 0x00000000FFFFFFFF;
    return x;
}

void unpack_3d(uint64_t d, uint64_t *x, uint64_t *y, uint64_t *z)
{
    *x = third_bits(d);
    *y = third_bits(d>>1);
    *z = third_bits(d>>2);
}

void z_order_3d(uint64_t dim, uint32_t *list)
{
    uint64_t i, x, y, z, idx = 0, extra = 0;

    for (i = 0; i < dim * dim * dim + extra; i++) {
        unpack_3d(i, &z, &y, &x);
        if (x >= dim || y >= dim || z >= dim) {
            extra++;
            continue;
        }
        list[idx++] = (uint32_t)(x*dim*dim + y*dim + z);

        #if VERBOSE
            printf("(%lu,%lu,%lu) -> %lu\n", x, y, z, x*dim*dim + y*dim + z);
        #endif
    }
}

uint64_t next_pow2(uint64_t x)
{
    uint64_t n = 0, xx = x;
    while (xx >>= 1) n++;
    if (1 << n != x) n++;
    return n;
}

int main (int argc, char **argv)
{
    uint64_t dim, len, np2;
    uint32_t *list = NULL;

    if (argc != 3) {
        printf("Usage: %s <2/3> <len>\n", argv[0]);
        exit(1);
    }

    dim = strtoul (argv[1], NULL, 0);
    len = strtoul (argv[2], NULL, 0);
    np2 = next_pow2(len);

    if (dim != 2 && dim != 3) {
        printf("Error: number of dimensions must be 2 or 3\n");
        exit(1);
    }

    if (len == 0) {
        printf("Error: dimension must be positive\n");
    }

    if ( (dim == 2 && np2 > 32) || (dim == 3 && np2 > 21) ) {
        printf("The dimension is too big to be mixed\n");
        exit(1);
    }


    if (dim == 2) {
        printf("The grid size is: (%lu,%lu)\n", len, len);

        list = (uint32_t*) malloc (sizeof(uint32_t) * len * len);
        assert(list);
        z_order_2d(len, list);

        for (int i = 0; i < len*len; i++) {
            printf("%u ", list[i]);
        }
        printf("\n");
    } else {
        printf("The grid size is: (%lu,%lu,%lu)\n", len, len,len);

        list = (uint32_t*) malloc (sizeof(uint32_t) * len * len * len);
        assert(list);
        z_order_3d(len, list);

        for (int i = 0; i < len*len*len; i++) {
            printf("%u ", list[i]);
        }
        printf("\n");
    }

    if (list) free(list);
}
