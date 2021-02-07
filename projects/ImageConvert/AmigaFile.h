#ifndef _AMIGAFILE_H
#define _AMIGAFILE_H

#include <memory.h>
#include <stdio.h>

#include <utils/system/endianness.h>
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

	void init(void);
	void clear(void);

	bool load(RGBAImage &image, Color32Palette &colors, const char *filename);

	bool save(RGBAImage const &image, Color32Palette &colors, const char *filename, uint16_t bitplanes, bool compress, bool interleave, bool saveColors);

	/**
	 * Convert an RGB color into an Amiga color.
	 */
	static uint16_t rgbToAmiga(const Color32 &color)
	{
		return ((color.getR() & 0xF0) << 4) + (color.getG() & 0xF0) + (color.getB() >> 4);
	}

	/**
	 * Convert the Amiga color value to an RGB value;
	 */
	static void amigaToRGB(uint16_t colorValue, Color32 &color)
	{
		color.setR(colorValue & (0x0f00) >> 4);
		color.setG((colorValue & 0xf0));
		color.setB((colorValue & 0x0f) << 4);
		color.setA(-1);
	}

	static uint16_t toAmigaEndian16(uint16_t value);
	static uint32_t toAmigaEndian32(uint32_t value);

private:
	uint32_t readImage(const char *filename, BitmapImage &bitmapImage, DynamicArray<uint16_t> &palette, DynamicArray<uint8_t> &planes);

	/**
	 * Convert the palette from RGB to an Amiga palette. Since distinct RGB colors may have the
	 * same value in Amiga, due to lesser bits, the resulting palette may be smaller then the input
	 * palette.
	 */
	SortedArray<uint16_t> toAmigaPalette(const Color32Palette &colors);

	/**
	 * Convert the color from an Amiga palette to an RGB value.
	 */
	bool rgbFromAmigaIndex(const DynamicArray<uint16_t> &palette, uint32_t index, Color32 &color);

	size_t amigaIndexFromRgb(const DynamicArray<uint16_t> &palette, const Color32 &color);

	bool saveHeader(FILE *file, const BitmapImage &bitmapImage, const SortedArray<uint16_t> &palette, bool saveColors) const;
	bool loadHeader(const char *filename, FILE *file, BitmapImage &bitmapImage, DynamicArray<uint16_t> &palette) const;

private:
	FILE *m_file;				// original image

	static bool m_littleEndian;
};

#endif // _AMIGAFILE_H
