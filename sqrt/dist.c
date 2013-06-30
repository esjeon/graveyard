
inline float
mysqrt (x)
	float x;
{
	int i = *(int*)&x;
	i = 0x1fbd1df5 + (i >> 1);
	x = *(float*)&i;
	return x;
}

__attribute__((always_inline))
inline float
distance (float cx, float cy, float x, float y)
{
	float X = x - cx;
	float Y = y - cy;
	return mysqrt(X*X + Y*Y);
}

__attribute__((always_inline))
inline float
distance_comp(float cx, float cy, float x, float y)
{
	float d = (cx - x) * (cx - x) + (cy - y) * (cy - y);
	int d2 = 0x1fbd1df5 + ((*(int*)&d) >> 1);
	return *(float*)&d2;
}

int
main ()
{
	volatile int i;
	volatile float d;

	for (i = 0; i < NUM; i ++) {
#ifdef COMP
		d = distance_comp(0, 0, i, i);
#else
		d = distance(0, 0, i, i);
#endif
	}

	return 0;
}

