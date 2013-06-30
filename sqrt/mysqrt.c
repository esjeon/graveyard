
#include <math.h>
#include <stdio.h>

volatile float d;

inline
float sqrss( float x )
{
	float z;
	__asm__(
			"rsqrtss %0, %%xmm0\n\t"
			"rcpss   %%xmm0, %%xmm0\n\t"
			"movss   %1, %%xmm0\n\t"
			: "=m"(z)
			: "m"(x)
		   );
	return z;
}

inline float
mysqrt (x)
	float x;
{
	int i = *(int*)&x;
	i = 0x1fbd1df5 + (i >> 1);
	x = *(float*)&i;
	return x;
}

int main ()
{
	int i;
	float x;
	for (i = 0; i < NUM; i ++) {
		x = (float)i * 0.0001f;
#ifdef MYSQRT
		d = mysqrt(x);
#else
		//d = sqrtf(x);
		d = sqrss(x);
#endif
	}

	return 0;
}
