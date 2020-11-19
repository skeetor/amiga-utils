
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <utils/algorithms/SmartPtr.h>
#include "AmigaFile.h"

bool AmigaFile::m_littleEndian = isLittleEndian();

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

uint16_t AmigaFile::toAmigaEndian16(uint16_t value)
{
	if (m_littleEndian)
		return flipEndian16(value);

	return value;
}

uint32_t AmigaFile::toAmigaEndian32(uint32_t value)
{
	if (m_littleEndian)
		return flipEndian32(value);

	return value;
}

SortedArray<uint16_t> AmigaFile::toAmigaPalette(const Color32Palette &colors)
{
	SortedArray<uint16_t> palette;
	palette.reserve(colors.size(), 0);

	for (const Color32Info &color : colors)
	{
		uint16_t col = rgbToAmiga(color.m_color);
		palette.add(col);
	}

	return palette;
}

bool AmigaFile::rgbFromAmigaIndex(const DynamicArray<uint16_t> &palette, uint32_t index, Color32 &color)
{
	if (index > palette.size())
		return false;

	uint16_t col = palette[index];

	amigaToRGB(col, color);

	return true;
}

size_t AmigaFile::amigaIndexFromRgb(const DynamicArray<uint16_t> &palette, const Color32 &color)
{
	uint16_t col = rgbToAmiga(color);

	for (size_t i = 0; i < palette.size(); i++)
	{
		if (palette[i] == col)
			return i;
	}

	return -1;
}

uint32_t AmigaFile::readImage(const char *filename, BitmapImage &bitmapImage, DynamicArray<uint16_t> &palette, DynamicArray<uint8_t> &planes)
{
	SmartPtr<FILE *> file(fopen(filename, "rb"), [](FILE *f) {fclose(f); });
	if (file == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to read file %s\n", filename);
		return 0;
	}

	if (!loadHeader(filename, file, bitmapImage, palette))
		return 0;

	uint16_t extraPixels = 0;
	extraPixels = bitmapImage.header.width % 16;
	if (extraPixels)
		extraPixels = 16 - extraPixels;

	uint32_t bytesPerRow = (bitmapImage.header.width+ extraPixels) / 8;
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

	// Get the colors from the file and convert it to an RGB palette with the same indexes.
	colors.sortByIndex();
	for (size_t i = 0; i < palette.size(); i++)
	{
		uint16_t col = palette[i];
		Color32 color;
		amigaToRGB(col, color);
		size_t index;
		colors.add(color, &index);
		colors[index].m_index = static_cast<uint32_t>(i);
	}

	colors.sortByColor();
	char buffer[200];
	char str[2];
	str[0] = 0;
	str[1] = 0;
	buffer[0] = 0;

	if (bi.header.flags & BIF_ZLIB)
	{
		strcat(buffer, str);
		strcat(buffer, "COMPRESSED");
		str[0] = '|';
	}

	if (bi.header.flags & BIF_INTERLEAVE)
	{
		strcat(buffer, str);
		strcat(buffer, "INTERLEAVE");
		str[0] = '|';
	}

	if (str[0] == 0)
		strcat(buffer, "NONE");

	fprintf(stdout, "[INF] Width: %u Height: %u Planes: %u Colors: %u Flags: %s\n", bi.header.width, bi.header.height, bi.header.planes, bi.header.colors, buffer);

	uint32_t bplLine = bytesPerRow;		// Distance to next row
	uint32_t nextBpl = bytesPerRow;		// Distance to next bitplane
	if (bi.header.flags & BIF_INTERLEAVE)
		bplLine *= bi.header.planes;
	else
		nextBpl *= bi.header.height;

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
				if (*b & bit)
					index |= (1 << i);

				b += nextBpl;
			}

			if (!rgbFromAmigaIndex(palette, index, *p))
			{
				fprintf(stderr, "[ERR] Image file invalid. Index %u exceeds maximum colors of %zu, stored in the image file\n", index, palette.size());
				return false;
			}

			size_t pos = 0;
			colors.add(*p, &pos);
			if (pos == -1)
			{
				fprintf(stderr, "[ERR] Unable to add color to palette\n");
				return false;
			}

			colors[pos].m_count++;

			p++;

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

bool AmigaFile::loadHeader(const char *filename, FILE *file, BitmapImage &bitmapImage, DynamicArray<uint16_t> &palette) const
{
	if (fread(&bitmapImage, sizeof(BitmapHeader), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Invalid header in file %s\n", filename);
		return false;
	}
	if (strcmp(bitmapImage.header.id, AMIGA_BPL_ID) != 0)
	{
		fprintf(stderr, "[ERR] Not an Amiga Raw Bitmap file %s\n", filename);
		return false;
	}

	if (bitmapImage.header.planes == 0)
	{
		fprintf(stderr, "[ERR] Invalid number of bitplanes\n");
		return false;
	}

	// Can't convert image if no colors are provided.
	if (bitmapImage.header.colors == 0)
	{
		fprintf(stderr, "[ERR] Image doesn't contain color information\n");
		return false;
	}

	palette.resize(bitmapImage.header.colors);
	if (palette == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to allocate palette entries\n");
		return false;
	}

	if (fread(&palette[0], palette.size() * sizeof(uint16_t), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to read color palette\n");
		return false;
	}

	for (size_t i = 0; i < palette.size(); i++)
		palette[i] = toAmigaEndian16(palette[i]);

	bitmapImage.header.width = toAmigaEndian16(bitmapImage.header.width);
	bitmapImage.header.height = toAmigaEndian16(bitmapImage.header.height);
	bitmapImage.header.frames = toAmigaEndian16(bitmapImage.header.frames);

	return true;
}

bool AmigaFile::saveHeader(FILE *file, const BitmapImage &bitmapImage, const SortedArray<uint16_t> &palette, bool saveColors) const
{
	BitmapImage bi = bitmapImage;

	bi.header.width = toAmigaEndian16(bi.header.width);
	bi.header.height = toAmigaEndian16(bi.header.height);
	bi.header.frames = toAmigaEndian16(bi.header.frames);

	if (fwrite(&bi, sizeof(BitmapHeader), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to write bitmap header\n");
		return false;
	}

	if (!saveColors)
		return true;

	DynamicArray<uint16_t> amigaPalette(palette.size(), true);
	for (size_t i = 0; i < palette.size(); i++)
		amigaPalette[i] = toAmigaEndian16(palette[i]);

	if (fwrite(&amigaPalette[0], sizeof(uint16_t)*amigaPalette.size(), 1, file) != 1)
	{
		fprintf(stderr, "[ERR] Unable to write colors\n");
		return false;
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

	// The width is rounded to the next 16 bits, so the bitmap is blitter friendly
	uint16_t extraPixels = 0;
	extraPixels = image.m_width % 16;
	if (extraPixels)
		extraPixels = 16 - extraPixels;

	bi.header.width = image.m_width;
	bi.header.height = image.m_height;
	bi.header.planes = (uint8_t)bitplanes;

	if (compress)
		bi.header.flags |= BIF_ZLIB;

	SmartPtr<FILE *> file(fopen(filename, "wb"), [](FILE *f){fclose(f);});
	if (file == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to open file %s\n", filename);
		return false;
	}

	if (!saveHeader(file, bi, palette, saveColors))
		return false;

	uint32_t bytesPerRow = (bi.header.width+extraPixels) / 8;
	uint32_t bplLine = bytesPerRow;		// Distance to next row
	uint32_t nextBpl = bytesPerRow;		// Distance to next bitplane
	if (interleave)
	{
		bi.header.flags |= BIF_INTERLEAVE;
		bplLine *= bitplanes;
	}
	else
		nextBpl *= bi.header.height;

	DynamicArray<uint8_t> planes(bytesPerRow * bi.header.height * bitplanes, true);
	if (planes == nullptr)
	{
		fprintf(stderr, "[ERR] Unable to allocate bitmap planes\n");
		return false;
	}

	const Color32 *p = image.m_pixels;
	uint8_t *bplBase = planes;

	for (uint16_t y = 0; y < image.m_height; y++)
	{
		uint8_t bit = 0x80;
		uint8_t *bpl = bplBase;

		for (uint16_t x = 0; x < image.m_width; x++)
		{
			size_t index = amigaIndexFromRgb(palette, *p++);
			if (index == -1)
			{
				fprintf(stderr, "[ERR] Unable to find color index\n");
				return false;
			}

			uint8_t *b = bpl;
			for (uint8_t i = 0; i < bi.header.planes; i++)
			{
				if (index & (static_cast<size_t>(1) << i))
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
