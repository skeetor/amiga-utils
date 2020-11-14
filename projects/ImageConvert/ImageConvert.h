#ifndef _IMAGECONVERT_H
#define _IMAGECONVERT_H

#include "gfxImage.h"
#include "Color32Info.h"

typedef enum
{
	GT_UNKNOWN,
	GT_AMIGA_RAW,
	GT_PNG_PALETTE,
	GT_PNG_RGB,
	GT_PNG_GRAY,

	GT_MAX
} GfxType;

class ImageConvert
{
public:
	ImageConvert(void)
	{
		init();
	}

	~ImageConvert(void)
	{
		clear();
	}

	void clear(void);

	bool (ImageConvert::*imageLoader)(void);
	bool (ImageConvert::*imageWriter)(void);

	bool loadImage(void)
	{
		return (this->*imageLoader)();
	}

	bool writeImage(void)
	{
		return (this->*imageWriter)();
	}

	/**
	 * Load the input file and convert it to the internal representation.
	 */
	bool loadPng(void);

	bool loadPaletteFile(const char *filename, Color32Palette &palette);

	/**
	 * Save the current image as PNG RGB
	 */
	bool writePngRGB(void);

	/**
	 * Save the current image as PNG Palette format
	 */
	bool writePngPalette(void);

	/**
	 * Load from amiga bitplane format
	 */
	bool loadAmiga(void);

	/**
	 * Write to amiga bitplane format
	 */
	bool writeAmiga(void);

	/**
	 * This will reduce the colors to the specified number.
	 * The palette will be overriden, and must be set to the desired target
	 * size.
	 */
	bool reduceColors(Color32Palette &palette);

	/**
	 * apply the new palette to the specified palette.
	 */
	bool remapColors(Color32Palette &palette);

protected:
	void init(void);

	/**
	 * Return the new color for the original color value. This will always
	 * return a valid pointer. This method will only return either the
	 * original or it's replacement. For writing to the target file
	 * getTargetColor() must be used.
	 */
	const Color32 *getActiveColor(Color32 const *color) const;

public:
	RGBAImage m_image;
	Color32Palette m_colorInfo;
	Color32Palette m_targetPalette;
	uint32_t m_reduceColors;					// Number of colors the user specified
	uint16_t m_bitplanes;						// Number of bitplanes

	GfxType m_inType : 8;
	GfxType m_outType : 8;

	const char *m_inputFilename;
	const char *m_outputFilename;
	const char *m_paletteDumpFilename;
	const char *m_targetPaletteFilename;
	const char *m_colorStatsFilename;

	bool m_sortByColor:1;
	bool m_saveColors:1;
	bool m_interleave:1;
	bool m_compress:1;
};

namespace PnnQuant
{
	/**
	 * Reduce the colors of an image. The palette has to be resized to the desried number of colors,
	 * with a maximum of 65535. It doesn't need to be initialized as it will be filled with the calculated
	 * colors by the algorithm. THe palette will be sorted by their color values.
	 *
	 * @param source Source image
	 * @param palette palette with the size set to the desired number of colors.
	 *
	 */
	bool QuantizeImage(RGBAImage &source, Color32Palette &palette);

	/**
	 * Remaps the colors of the image to a new palette. The palette must be initialized with the desired
	 * colors. If the color fro the image is not in the palette, the closest matching color will be
	 * taken.
	 */
	bool RemapImage(RGBAImage &source, const Color32Palette &palette);
}

#endif // _IMAGECONVERT_H
