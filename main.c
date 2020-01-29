// Author: Patrick Lavin
// Date:   14 January 2020
//
// Driver code for the morton.c functions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "morton.h"

int main (int argc, char **argv)
{
    uint64_t dim, len, block = 1;
    uint32_t *list = NULL;

    if (argc < 3) {
        printf("Usage: %s <2/3> <len> [block]\n", argv[0]);
        printf("1st arg: 2D or 3D pattern\n");
        printf("2nd arg: length of one dimension\n");
        printf("3rd arg: block size (block size must divide length) [default: 1, no blocking]\n");
        exit(1);
    }

    dim = strtoul(argv[1], NULL, 0);
    len = strtoul(argv[2], NULL, 0);

    if (dim != 2 && dim != 3) {
        printf("Error: number of dimensions must be 2 or 3\n");
        exit(1);
    }

    if (argc > 3) {
        block = strtoul(argv[3], NULL, 0);
    }


    if (dim == 2) {
        printf("The grid size is: (%lu,%lu)\n", len, len);

        list = z_order_2d(len, block);

        if (!list) {
            printf("Call to z_order failed\n");
            exit(1);
        }

        for (int i = 0; i < len*len; i++) {
            printf("%u ", list[i]);
        }
        printf("\n");
    } else {
        printf("The grid size is: (%lu,%lu,%lu)\n", len, len,len);

        list = z_order_3d(len, block);

        if (!list) {
            printf("Call to z_order failed\n");
            exit(1);
        }

        for (int i = 0; i < len*len*len; i++) {
            printf("%u ", list[i]);
        }
        printf("\n");
    }

    if (list) free(list);
}
