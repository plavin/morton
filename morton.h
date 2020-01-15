// Author: Patrick Lavin
// Date:   14 January 2020
//
// This code prints a 2d or 3d morton ordering

#ifndef MORTON_H
#define MORTON_H
uint64_t next_pow2(uint64_t x);
uint32_t *z_order_2d(uint64_t dim);
uint32_t *z_order_3d(uint64_t dim);
#endif
