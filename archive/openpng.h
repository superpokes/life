
#include "png.h"

typedef struct {
	GLubyte * pxdata;
	GLuint width;
	GLuint height;
	bool hasAlpha;
} loaded_png;

loaded_png * loadPNGImage(const char * path_to_file)
{
	loaded_png * ret = (loaded_png *)std::malloc(sizeof(loaded_png));

	FILE * f = std::fopen(path_to_file, "rb");
	if (f == NULL) {
		return NULL;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
		NULL, NULL);
	if (png_ptr == NULL) {
		fclose(f);
		return NULL;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(f);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return NULL;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (end_info == NULL) {
		fclose(f);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return NULL;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(f);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return NULL;
	}

	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 0);

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16
		| PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	ret->width = width;
	ret->height = height;
	ret->hasAlpha = color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_MASK_ALPHA;

	uint32_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	ret->pxdata = (GLubyte *)malloc(row_bytes * ret->height);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	for (GLuint i = 0; i < ret->height; i++) {
		memcpy(ret->pxdata + (row_bytes * (ret->height - 1 - i)), row_pointers[i],
			row_bytes);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(f);

	return ret;
}
