// Author: Patrick Lavin
// This code prints a 2d morton ordering
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

// Taken from https://stackoverflow.com/a/30562230
uint32_t even_bits(uint64_t x)
{
    x = x & 0x5555555555555555;
    x = (x | (x >> 1))  & 0x3333333333333333;
    x = (x | (x >> 2))  & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4))  & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8))  & 0x0000FFFF0000FFFF;
    x = (x | (x >> 16)) & 0x00000000FFFFFFFF;
    return (uint32_t)x;
}

// Taken from https://stackoverflow.com/a/30562230
void d2xy_morton(uint64_t d, uint32_t *x, uint32_t *y)
{
    *x = even_bits(d);
    *y = even_bits(d >> 1);
}

// TODO: Make everything 64 bits, manually check that dim fits
uint64_t morton3(uint64_t x) {
    x = x & 0x9249249249249249;
    x = (x | (x >> 2))  & 0x30c30c30c30c30c3;
    x = (x | (x >> 4))  & 0xf00f00f00f00f00f;
    x = (x | (x >> 8))  & 0x00ff0000ff0000ff;
    x = (x | (x >> 16)) & 0xffff00000000ffff;
    x = (x | (x >> 32)) & 0x00000000ffffffff;
    return x;
}
/*
uint64_t bits;
uint64_t x = morton3(bits)
uint64_t y = morton3(bits>>1)
uint64_t z = morton3(bits>>2)
*/

void z_ordering_2d(uint32_t dim)
{
    uint32_t x, y, extra = 0;
    uint64_t i;

    for (i = 0; i < dim * dim + extra; i++) {
        d2xy_morton(i, &y, &x);
        if (x >= dim || y >= dim) {
            extra++;
            continue;
        }
        printf("(%u,%u) -> %lu\n", x, y, ((uint64_t)x)*dim + y);
    }
}

void z_order_list(uint32_t dim, uint64_t *list)
{
    uint32_t x, y, idx = 0, extra = 0;
    uint64_t i;

    for (i = 0; i < dim * dim + extra; i++) {
        d2xy_morton(i, &y, &x);
        if (x >= dim || y >= dim) {
            extra++;
            continue;
        }
        list[idx++] = ((uint64_t)x)*dim + y;
    }
}

int main (int argc, char **argv)
{
    uint64_t dim_l;
    uint32_t dim;

    // Check args
    if (argc != 2) {
        printf("Usage: %s <dim>\n", argv[0]);
        exit(1);
    }

    dim_l = strtoul (argv[1], NULL, 0);
    dim = (uint32_t) dim_l;

    if (dim_l > dim) {
        printf("Error: Dimension is too big to be represented by 32 bits\n");
        exit(1);
    }

    if (dim == 0) {
        printf("Error: Dimension must be positive\n");
    }

    // Print ordering as x, y pairs
    printf("The grid size is: (%u,%u)\n", dim, dim);
    z_ordering_2d(dim);

    // Get ordering as indices into 1d array of size dim x dim
    uint64_t *list = (uint64_t*) malloc (sizeof(uint64_t) * dim * dim);
    assert(list);
    z_order_list(dim, list);

    // Print list
    for (int i = 0; i < dim*dim; i++) {
        printf("%lu ", list[i]);
    }
    printf("\n");

    free(list);

}
