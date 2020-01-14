#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


unsigned morton_1(unsigned long x)
{
    x = x & 0x5555555555555555;
    x = (x | (x >> 1))  & 0x3333333333333333;
    x = (x | (x >> 2))  & 0x0F0F0F0F0F0F0F0F;
    x = (x | (x >> 4))  & 0x00FF00FF00FF00FF;
    x = (x | (x >> 8))  & 0x0000FFFF0000FFFF;
    x = (x | (x >> 16)) & 0x00000000FFFFFFFF;
    return (unsigned)x;
}

void d2xy_morton(unsigned long d, unsigned *x, unsigned *y)
{
    *x = morton_1(d);
    *y = morton_1(d >> 1);
}

void z_ordering_2d_2(unsigned dim, unsigned npow2)
{
    unsigned ii, jj;
    unsigned extra = 0;
    for (unsigned i = 0; i < dim * dim + extra; i++) {
        d2xy_morton(i, &jj, &ii);
        if (ii >= dim || jj >= dim) {
            extra++;
            continue;
        }
        printf("(%u,%u) -> %lu\n", ii, jj, ((unsigned long)ii)*dim + jj);
    }
}

unsigned long *z_order_list(unsigned dim)
{
    unsigned npow2 = 0;
    unsigned dim_copy = dim;
    while (dim_copy >>= 1) npow2++;
    if (1<<npow2 != dim) npow2++;
    unsigned long *list = (unsigned long*) malloc (sizeof(unsigned long) * dim * dim);
    assert(list);

    unsigned ii, jj;
    unsigned extra = 0;
    unsigned long idx = 0;
    for (unsigned long i = 0; i < dim * dim + extra; i++) {
        d2xy_morton(i, &jj, &ii);
        if (ii >= dim || jj >= dim) {
            extra++;
            continue;
        }
        list[idx++] = ((unsigned long)ii)*dim + jj;
    }
     return list;
}

int main (int argc, char **argv)
{
    unsigned long dim_l;
    unsigned dim, dim_copy, npow2;

    /*
    unsigned x1 = 0;
    unsigned x2 = 2;
    printf("(%u,%u)->%lu\n", x1, x2, mix_2d(x2, x1));
    exit(0);
    */

    if (argc != 2) {
        printf("Usage: %s <dim>\n", argv[0]);
        exit(1);
    }

    dim_l = strtoul (argv[1], NULL, 0);
    dim = (unsigned) dim_l;

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
    unsigned long *list = z_order_list(dim);

    for (int i = 0; i < dim*dim; i++) {
        printf("%lu ", list[i]);
    }
    printf("\n");

}
