# Morton
This is a prototype of code that will be put into Spatter. It produces a morton- or (z-) ordering, even for non-power-of-two sizes. For such sizes, it will simply use the next power of two curve, but skip points which would lie outside the domain. 

## Usage
There are two required arguments, the number of dimensions (2, or 3) and the length of each dimension. 
```
./morton <2/3> <len>
```
