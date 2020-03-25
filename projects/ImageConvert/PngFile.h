#ifndef _PNGFILE_H
#define _PNGFILE_H

#include <functional>

#include <utils/tools/png/amiga_png.h>

#include "ImageFile.h"

typedef struct _ImageProcessor ImageProcessor;

typedef enum
{
	PT_PALETTE,
	PT_RGB,
	PT_GRAY,

	PT_MAX
} PngType;

typedef struct _PngFile : public ImageFile
{
	typedef std::function<uint8_t *(uint8_t *fileData, Color32 &image, bool alpha)> ColorConverter;

	FILE *m_file;				// original image
	png_structp m_handle;
	png_infop m_info;
	png_colorp m_palette;
	bool m_readMode : 1;

	void init(void);
	void clear(void);

	bool load(RGBAImage &image, Color32Palette &colors, const char *filename);

	bool saveRGBFormat(RGBAImage const &image, Color32Palette &colors, const char *filename);
	bool savePaletteFormat(RGBAImage const &image, Color32Palette &colors, const char *filename);

private:
	bool open(const char *filename, bool bRead);
	bool initWrite(RGBAImage const &image, const char *filename, int imgType, int depth);
	bool writePaletteColors(Color32Palette &colorInfo);
	bool writePaletteRows(RGBAImage const &image, Color32Palette &colors);

	bool parseImage(RGBAImage &image, Color32Palette &colors, bool alpha, ColorConverter colorConverter);
	bool parseRGB(RGBAImage &image, Color32Palette &colors, bool alpha);
	bool parsePalette(RGBAImage &image, Color32Palette &colors);
} PngFile;

#endif // _PNGFILE_H
