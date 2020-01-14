#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

uint32_t morton_1(uint64_t x)
{
    x = x & 0x5555555555555555;
    x = (x | (x >> 1))  & 0x3333333333333333;
    x = (x | (x >> 2))  & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4))  & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8))  & 0x0000FFFF0000FFFF;
    x = (x | (x >> 16)) & 0x00000000FFFFFFFF;
    return (uint32_t)x;
}

void d2xy_morton(uint64_t d, uint32_t *x, uint32_t *y)
{
    *x = morton_1(d);
    *y = morton_1(d >> 1);
}

void z_ordering_2d_2(uint32_t dim, uint32_t npow2)
{
    uint32_t ii, jj;
    uint32_t extra = 0;
    for (uint32_t i = 0; i < dim * dim + extra; i++) {
        d2xy_morton(i, &jj, &ii);
        if (ii >= dim || jj >= dim) {
            extra++;
            continue;
        }
        printf("(%u,%u) -> %lu\n", ii, jj, ((uint64_t)ii)*dim + jj);
    }
}

uint64_t *z_order_list(uint32_t dim)
{
    uint32_t npow2 = 0;
    uint32_t dim_copy = dim;
    while (dim_copy >>= 1) npow2++;
    if (1<<npow2 != dim) npow2++;
    uint64_t *list = (uint64_t*) malloc (sizeof(uint64_t) * dim * dim);
    assert(list);

    uint32_t ii, jj;
    uint32_t extra = 0;
    uint64_t idx = 0;
    for (uint64_t i = 0; i < dim * dim + extra; i++) {
        d2xy_morton(i, &jj, &ii);
        if (ii >= dim || jj >= dim) {
            extra++;
            continue;
        }
        list[idx++] = ((uint64_t)ii)*dim + jj;
    }
     return list;
}

int main (int argc, char **argv)
{
    uint64_t dim_l;
    uint32_t dim, dim_copy, npow2;

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

    dim_copy = dim;
    npow2 = 0;
    while (dim_copy >>= 1) npow2++;
    if (1<<npow2 != dim) npow2++;

    printf("The grid size is: (%u,%u)\n", dim, dim);
    printf("The number of bits needed to store dimension is: %u\n", npow2);

    z_ordering_2d_2(dim, npow2);
    uint64_t *list = z_order_list(dim);

    for (int i = 0; i < dim*dim; i++) {
        printf("%lu ", list[i]);
    }
    printf("\n");

    free(list);

}
