# Morton
This is a prototype of code that will be put into Spatter. It produces a [morton- or (z-) ordering](https://en.wikipedia.org/wiki/Z-order_curve), even for non-power-of-two sizes. For such sizes, it will simply use the next power-of-two curve, but skip points which would lie outside the domain. 

## Driver Usage
We provide a main function for testing the code, in main.c. You can compile it with `make`. This will produce an executable called `morton`. There are two required arguments, the number of dimensions (2 or 3) and the length of each dimension. 
```
./morton <2/3> <len>
```

## Usage in Other Projects
You can include `morton.h` and `morton.c` in your project to access the two functions `z_order_2d` and `z_order_3d`. These take a single argument, the dimension length. They allocate a list containing the morton ordering of length `len*len` or `len*len*len`, respectively. On success, a pointer to this list is returned, or `NULL` is returned on failure. If you wish to gain more info on why the function returned `NULL`, change the define at the top of `morton.c` to `#define MORTON_VERBOSE 1`. 

## Notes
As the indices will be mixed into a single 64-bit variable (`uint64_t`), the indices must be no larger than 32 bits in 2D, or 21 bits in 3D. If you use a non-power-of-two length, then the next largest power of two will be used to determine if your chosen dimension length can be mixed in 64 bits. 

Due to the above restriction, the largest number of bits needed to store an index is 32. Thus the list returned from `z_order_*` has type `uint32_t*`.

