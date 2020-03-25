#ifndef _UTILSLIB_IMAGE_I
#define _UTILSLIB_IMAGE_I

#include <utils/utils_def.h>

#define AMIGA_BPL_ID "ABMI"

typedef enum
{
	BIF_NONE = 0,

	BIF_COMPRESSED		= (1 << 0),			// Bitmap data is zlib compressed
	BIF_INTERLEAVE		= (1 << 1),

	BIF_MAX
} BitmapImageFlags;

#pragma pack(push,1)
typedef struct _BitmapHeader
{
	char id[5];
	uint8_t flags;
	uint16_t width;
	uint16_t height;
	uint8_t colors;							// Number of color entries. May be 0 if colors are not stored with the image data
	uint8_t planes;

	void init(void)
	{
		memcpy(id, AMIGA_BPL_ID, 5);
		width = 0;
		height = 0;
		flags = BIF_NONE;
		colors = 0;
		planes = 0;
	}
} BitmapHeader;

typedef struct _BitmapImage
{
	BitmapHeader header;
	uint8_t *bitmap[8];

	void init(void)
	{
		header.init();

		for (uint8_t i = 0; i < sizeof(bitmap) / sizeof(uint8_t *); i++)
			bitmap[i] = nullptr;
	}
} BitmapImage;
#pragma pack(pop)

/**
 ************************************************************
 * Load an (PNG) image from a file. Returns true if the file
 * was successfully loaded. The image structure will be
 * initialized by the loader.
 *
 * INPUT:
 *		image
 *		filename 
 *
 * RETURN: True if the image was successfully loaded. If false
 * is returned, the image structure may not be accessed as its
 * state is undefined.
 *
 ************************************************************
 */
//C_FUNCTION bool LoadPNG(BitmapImage *image REGISTER_PARAM(a0), const char *filename REGISTER_PARAM(a1));

#endif // _UTILSLIB_IMAGE_I
