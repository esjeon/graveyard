
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int
ppm_write (path, width, height, rgb, maxval, plain)
	const char *path;
	unsigned int width, height;
	uint8_t rgb[][3];
	uint8_t maxval;
	bool plain;
{
	FILE *fp;
	int i, c, len;

	fp = fopen(path, "w");
	if (!fp)
		return 1;

	fprintf(fp, "P%d %d %d %d\n",
			((plain)? 3: 6), width, height, maxval);

	len = width * height;
	if (plain)
		for (i = 0, c = 0; i < len; i ++) {
			fprintf (fp, "%3d %3d %3d", rgb[i][0], rgb[i][1], rgb[i][2]);
			if ((++c) == width)
				c = 0, fputc('\n', fp);
			else
				fputc(' ', fp);
		}
	else
		for (i = 0; i < width * height; i ++)
			fwrite(rgb[i], sizeof(uint8_t), 3, fp);

	fclose (fp);

	return 0;
}

/* test main */
int main()
{
	uint8_t rgb[255*255][3];
	int i, j;
	for (i = 0; i < 255; i ++) {
		for (j = 0; j < 255; j ++) {
			rgb[255*i+j][0] = i;
			rgb[255*i+j][1] = j;
			rgb[255*i+j][2] = (i*j) % 256;
		}
	}

	ppm_write("raw.ppm", 255, 255, rgb, 255, false);
	ppm_write("plain.ppm", 255, 255, rgb, 255, true);

	return 0;
}

