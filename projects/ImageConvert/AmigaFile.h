#ifndef _AMIGAFILE_H
#define _AMIGAFILE_H

#include <memory.h>
#include <stdio.h>

#include <utils/gfx/image.h>
#include <utils/algorithms/SortedArray.h>
#include "ImageFile.h"

class AmigaFile 
: public ImageFile
{
public:
	AmigaFile(void)
	: m_file(nullptr)
	{
		init();
	}

	~AmigaFile(void)
	{
		clear();
	}

	FILE *m_file;				// original image

	void init(void);
	void clear(void);

	bool load(RGBAImage &image, Color32Palette &colors, const char *filename);

	bool save(RGBAImage const &image, Color32Palette &colors, const char *filename, uint16_t bitplanes, bool compress, bool interleave, bool saveColors);

	/**
	 * Convert an RGB color into an Amiga color.
	 */
	uint16_t toAmigaColor(const Color32 &color)
	{
		return ((color.getR() & 0xF0) << 4) + (color.getG() & 0xF0) + (color.getB() >> 4);
	}

private:
	uint32_t readImage(const char *filename, BitmapImage &bitmapImage, DynamicArray<uint16_t> &palette, DynamicArray<uint8_t> &planes);

	SortedArray<uint16_t> toAmigaPalette(const Color32Palette &colors);
	bool rgbFromIndex(const DynamicArray<uint16_t> &palette, uint32_t index, Color32 &color);
};

#endif // _AMIGAFILE_H
