
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <utils/algorithms/SmartPtr.h>
#include "AmigaFile.h"

void AmigaFile::init(void)
{
	m_file = nullptr;
}

void AmigaFile::clear(void)
{
	if (m_file)
		fclose(m_file);

	init();
}

SortedArray<uint16_t> AmigaFile::toAmigaPalette(const Color32Palette &colors)
{
	SortedArray<uint16_t> palette;
	palette.reserve(colors.size());

	size_t index = 0;
	for (const Color32Info &color : colors)
	{
		uint16_t col =  ((color.m_color.getR() & 0xF0) << 4) + (color.m_color.getG() & 0xF0) + (color.m_color.getB() >> 4);
		palette.add(col, &index);
	}

	return palette;
}

bool AmigaFile::rgbFromIndex(const DynamicArray<uint16_t> &palette, uint32_t index, Color32 &color)
{
	if (index >= palette.size())
		return false;

	uint16_t col = palette[index];

	color.setR(col & (0x0f00) >> 4);
	color.setG((col & 0xf0));
	color.setB((col & 0x0f) << 4);
	color.setA(-1);

	return true;
}

uint32_t AmigaFile::readImage(const char *filename, BitmapImage &bitmapImage, DynamicArray<uint16_t> &palette, DynamicArray<uint8_t> &planes)
{
	SmartPtr<FILE> file(fopen(filename, "rb"), [](FILE *f) {fclose(f); });
	if (file == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to read file %s\n", filename);
		return 0;
	}

	if (fread(&bitmapImage, sizeof(BitmapHeader), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Invalid header in file %s\n", filename);
		return 0;
	}

	if (strcmp(bitmapImage.header.id, AMIGA_BPL_ID) != 0)
	{
		fprintf(stderr, "[ERR] Not an Amiga Raw Bitmap file %s\n", filename);
		return 0;
	}

	if (bitmapImage.header.planes == 0)
	{
		fprintf(stderr, "[ERR] Invalid number of bitplanes\n");
		return 0;
	}

	// Can't convert image if no colors are provided.
	if (bitmapImage.header.colors == 0)
	{
		fprintf(stderr, "[ERR] Image doesn't contain color information\n");
		return 0;
	}

	palette.resize(bitmapImage.header.colors);
	if (palette == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to allocate palette entries\n");
		return 0;
	}

	if (fread(&palette[0], palette.size() * sizeof(uint16_t), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to read color palette\n");
		return 0;
	}

	uint32_t bytesPerRow = bitmapImage.header.width / 8;
	if (bitmapImage.header.width % 8)
		bytesPerRow++;

	planes.resize(bytesPerRow * bitmapImage.header.height * bitmapImage.header.planes);
	if (planes == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to allocate bitmap planes\n");
		return 0;
	}

	if (fread(planes, planes.size(), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to read image planes\n");
		return 0;
	}

	return bytesPerRow;
}

bool AmigaFile::load(RGBAImage &image, Color32Palette &colors, const char *filename)
{
	BitmapImage bi;
	bi.init();
	DynamicArray<uint16_t> palette;
	DynamicArray<uint8_t> planes;

	uint32_t bytesPerRow;
	if ((bytesPerRow = readImage(filename, bi, palette, planes)) == 0)
		return false;

	if (!image.createImage(bi.header.width, bi.header.height))
	{
		fprintf(stderr, "[ERR] Unable to allocate bitmap\n");
		return false;
	}

	uint32_t bplLine = bytesPerRow;		// Distance to next row
	uint32_t nextBpl = bytesPerRow;		// Distance to next bitplane
	if (bi.header.flags & BIF_INTERLEAVE)
		bplLine *= bi.header.planes;
	else
		nextBpl *= image.m_height;

	Color32 *p = image.m_pixels;
	uint8_t *bplBase = planes;

	for (uint16_t y = 0; y < image.m_height; y++)
	{
		uint8_t *bpl = bplBase;
		uint8_t bit = 0x80;

		for (uint16_t x = 0; x < image.m_width; x++)
		{
			uint8_t *b = bpl;
			uint32_t index = 0;

			for (uint8_t i = 0; i < bi.header.planes; i++)
			{
				index <<= 1;
				if (*b & bit)
					index |= 1;

				b += nextBpl;
			}

			if (!rgbFromIndex(palette, index, *p++))
			{
				fprintf(stderr, "[ERR] Image file invalid. Index %u exceeds maximum supported colors of %u for %u bitplanes\n", index, 1 << bi.header.planes, bi.header.planes);
				return false;
			}

			index = 0;

			bit >>= 1;
			if (!bit)
			{
				bit = 0x80;
				bpl++;
			}
		}

		bplBase += bplLine;
	}
	
	return true;
}

bool AmigaFile::save(RGBAImage const &image, Color32Palette &colors, const char *filename, uint16_t bitplanes, bool compress, bool interleave, bool saveColors)
{
	const SortedArray<uint16_t> &palette = toAmigaPalette(colors);
	BitmapImage bi;
	bi.init();

	if (saveColors)
		bi.header.colors = static_cast<uint8_t>(palette.size());

	bi.header.width = image.m_width;
	bi.header.height = image.m_height;
	bi.header.planes = (uint8_t)bitplanes;

	if (compress)
		bi.header.flags |= BIF_COMPRESSED;

	uint32_t bytesPerRow = image.m_width / 8;
	if (image.m_width % 8)
		bytesPerRow++;

	uint32_t bplLine = bytesPerRow;		// Distance to next row
	uint32_t nextBpl = bytesPerRow;		// Distance to next bitplane
	if (interleave)
	{
		bi.header.flags |= BIF_INTERLEAVE;
		bplLine *= bitplanes;
	}
	else
		nextBpl *= image.m_height;

	DynamicArray<uint8_t> planes(bytesPerRow * image.m_height * bitplanes, true);
	if (planes == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to allocate bitmap planes\n");
		return false;
	}

	SmartPtr<FILE> file(fopen(filename, "wb"), [](FILE *f) {fclose(f);});
	if (file == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to open file %s\n", filename);
		return false;
	}

	if (fwrite(&bi, sizeof(BitmapHeader), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to write bitmap header\n");
		return false;
	}

	if (saveColors)
	{
		if (fwrite(&palette[0], sizeof(uint16_t)*palette.size(), 1, file) != 1)
		{
			fprintf(stderr, "[ERR] Unable to write colors\n");
			return false;
		}
	}

	const Color32 *p = image.m_pixels;
	uint8_t *bplBase = planes;
	uint8_t startMask = 1 << (bitplanes - 1);

	for (uint16_t y = 0; y < image.m_height; y++)
	{
		uint8_t bit = 0x80;
		uint8_t *bpl = bplBase;

		for (uint16_t x = 0; x < image.m_width; x++)
		{
			size_t index = palette.find(toAmigaColor(*p++));
			uint8_t *b = bpl;

			for(uint8_t mask = startMask; mask; mask >>= 1)
			{
				if (index & mask)
					*b |= bit;

				b += nextBpl;
			}

			bit >>= 1;
			if (!bit)
			{
				bpl++;
				bit = 0x80;
			}
		}

		bplBase += bplLine;
	}

	if (fwrite(&planes[0], planes.size(), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to write bitmap data\n");
		return false;
	}

	return true;
}
