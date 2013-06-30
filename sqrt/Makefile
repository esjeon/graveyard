CC=gcc

all:
	$(CC) -DNUM=100000000 -DMYSQRT -O3 mysqrt.c -o mysqrt
	$(CC) -DNUM=100000000 -lm      -O3 mysqrt.c -o sqrt
	$(CC) -DNUM=100000000 -DMYSQRT -O3 mysqrt.c -S -o mysqrt.s
	$(CC) -DNUM=100000000 -lm      -O3 mysqrt.c -S -o sqrt.s
	
	$(CC) -march=native -DNUM=500000000 -O3        dist.c -o dosa
	$(CC) -march=native -DNUM=500000000 -O3 -DCOMP dist.c -o cosa
	$(CC) -march=native -DNUM=500000000 -O3        dist.c -S -o dosa.s
	$(CC) -march=native -DNUM=500000000 -O3 -DCOMP dist.c -S -o cosa.s

clean:
	rm -f mysqrt sqrt dosa cosa 
	rm -f mysqrt.s sqrt.s dosa.s cosa.s

