
unmanaged/sqrt
==============

mysqrt.c
--------
 - I implemented my own fast-sqrt approx. function `mysqrt`
 - I also implemented my own SSE version of sqrt `sqrss`
   Note that glibc already has SSE version of sqrt, but I don't know if it's possible to inline it or not.

dist.c
------
 - compute the distance b/w two points using mysqrt function
 - see how this function is optimized. it's quite interesting
   - **dosa** yields less number of ops, but the binary is slower.
 - try **clang**. its optimizer is quite impressive.

