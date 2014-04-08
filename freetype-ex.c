/* freetype-ex.c: Some stuffs from freetype tutorial
 *
 * compile with
 *   ${CC} `pkg-config --cflags --libs freetype2` freetype-ex.c
 *
 */

#include <ft2build.h>
#include FT_FREETYPE_H

#define FONT "/usr/share/fonts/truetype/nanum/NanumGothic.ttf"

FT_Library library;

void
dump_bitmap(bm)
	FT_Bitmap *bm;
{
	int i, j, p;
	
	printf("=== Glyph Begin ===\n");
	printf("dimension: %dx%d\n", bm->rows, bm->width);
	printf("pitch: %d\n", bm->pitch);
	printf("num_grays: %d\n", bm->num_grays);

	p = 0;
	for(i = 0; i < bm->rows; i ++) {
		for(j = 0; j < bm->width; j ++)
			printf("%4d", bm->buffer[p++]);
		printf("\n");
	}

	printf("=== Glyph End ===\n");
}


int
main(argc, argv)
	int argc;
	char *argv[];
{
	FT_Error err;
	FT_Face face;
	FT_UInt idx;

	err = FT_Init_FreeType(&library);
	if(err) {
		fprintf(stderr, "failed to init freetype\n");
		exit(1);
	}

	/* TODO: 3rd param = face_index
	 *       there should be a way to get this */
	err = FT_New_Face(library, FONT, 0, &face);
	if(err == FT_Err_Unknown_File_Format) {
		fprintf(stderr, "unsupported file type\n");
	}
	else if(err) {
		fprintf(stderr, "failed to load the font\n");
		exit(1);
	}

#define show(field, type) \
	printf("face->"#field" : " type "\n", face->field)

	show(num_glyphs, "%ld");
	show(face_flags, "0x%lx");
	show(num_fixed_sizes, "%d");
#undef show

	err = FT_Set_Pixel_Sizes(face, 0, 16);
	if(err) {
		fprintf(stderr, "failed to set the font pixel size\n");
		exit(1);
	}

	idx = FT_Get_Char_Index(face, 0xAC00); // 가
	if(idx == 0) {
		fprintf(stderr, "WARNING: no glyph\n");
	}
	printf("glyph index: %d\n", idx);

	err = FT_Load_Glyph(face, idx, 0);
	if(err) {
		fprintf(stderr, "failed to load glyph\n");
		exit(1);
	}

	err = FT_Render_Glyph(face->glyph, 0);
	if(err) {
		fprintf(stderr, "failed to render glyph\n");
		exit(1);
	}

	dump_bitmap(&face->glyph->bitmap);
}

