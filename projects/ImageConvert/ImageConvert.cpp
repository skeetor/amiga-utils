#include "utils/algorithms/DynamicArray.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <memory.h>

#include "utils/string/numbers.h"
#include "ImageConvert.h"
#include "AmigaFile.h"
#include "PngFile.h"

void ImageConvert::init(void)
{
	m_inType = GT_UNKNOWN;
	m_outType = GT_UNKNOWN;
	m_reduceColors = invalid_t;
	m_bitplanes = invalid_t;

	imageLoader = &ImageConvert::loadPng;
	imageWriter = &ImageConvert::writePngPalette;

	m_inputFilename = nullptr;
	m_outputFilename = nullptr;
	m_paletteDumpFilename = nullptr;
	m_colorStatsFilename = nullptr;
	m_targetPaletteFilename = nullptr;

	m_sortByColor = true;
	m_saveColors = true;
	m_interleave = false;
	m_compress = false;
}

void ImageConvert::clear(void)
{
}

bool ImageConvert::loadPaletteFile(const char *filename, Color32Palette &palette)
{
	bool rc = true;
	palette.clear();
	palette.sortByIndex();

	if (filename)
	{
		FILE *f = fopen(filename, "rb");
		if (f)
		{
			uint32_t paletteIndex = 0;
			uint32_t lineNumber = 0;

			DynamicArray<char> line(128u);
			if (line.ptr() == nullptr)
			{
				printf("[ERR] Unable to allocate memory for line reading\n");
				rc = false;
			}
			else
			{
				line[line.last()] = 0;
				line.resize(line.last() - 1);

				while (fgets(line.ptr(), static_cast<int>(line.size()), f) != nullptr)
				{
					char *p = line.ptr();
					size_t index = strcspn(p, "\r\n");
					char *e = &p[index];
					if (*e == 0)
					{
						// A very long line, so read until we find the EOL or EOF
						line.resize(line.size() * 2);
						line[line.last()] = 0;
						line.resize(line.last() - 1);
						continue;
					}

					lineNumber++;

					*e = 0;
					p = line.ptr();

					// Ignore comments
					if (*p == '#')
						continue;

					// uint32_t index = strcspn(p, "0123456789");
					// Skip whitespaces
					index = strspn(p, " \t");

					p = &p[index];
					if (p >= e || *p == 0)			// Empty line
						continue;

					char *pe;
					index = strtoul(p, &pe, 10);
					p = pe;
					if (p >= e || *p != ':')		// Incomplete
					{
						printf("[ERR] Incomplete line %lu\n", lineNumber);
						rc = false;
						break;
					}

					p++;
					index = strspn(p, " \t");
					p = &p[index];
					if (p >= e || *p == 0)			// Incomplete
					{
						printf("[ERR] Incomplete line %lu\n", lineNumber);
						rc = false;
						break;
					}

					uint32_t colorValue = strtoul(p, &pe, 16);
					if (p == pe)					// Invalid number
					{
						printf("[ERR] Invalid number on line %lu\n", lineNumber);
						rc = false;
						break;
					}

					Color32Info color = colorValue;
					color.m_index = paletteIndex++;
					palette.add(color);
				}
			}
			fclose(f);
		}
		else
		{
			printf("[ERR] Unable to open file %s\n", filename);
			rc = false;
		}
	}

	return rc;
}

bool ImageConvert::loadPng(void)
{
	PngFile file;
	file.init();

	if (!file.load(m_image, m_colorInfo, m_inputFilename))
	{
		file.clear();
		return false;
	}

	file.clear();
	printf("[INF] %s sucessfully loaded\n", m_inputFilename);

	return true;
}

bool ImageConvert::writePngPalette(void)
{
	PngFile file;
	file.init();

	RGBAImage &image = m_image;
	Color32Palette &colors = m_colorInfo;
	Color32Palette &targetColors = m_targetPalette;
	Color32Palette *palette = &colors;

	// If the user specified a larger palette then we can support, we can not
	// automatically determine which colors should actually be used.
	if (targetColors.size() > 256)
	{
		printf("[ERR] specified target palette contains more than 256 colors!\n");
		return false;
	}

	if (targetColors.empty())
	{
		if (colors.size() > 256)
		{
			colors.resize(256);
			reduceColors(colors);
		}
	}
	else
		palette = &m_targetPalette;

	if (!file.savePaletteFormat(image, *palette, m_outputFilename))
	{
		file.clear();
		return false;
	}

	file.clear();
	printf("[INF] %s sucessfully written\n", m_outputFilename);

	return true;
}

bool ImageConvert::writePngRGB(void)
{
	PngFile file;
	file.init();

	if (!file.saveRGBFormat(m_image, m_colorInfo, m_outputFilename))
	{
		file.clear();
		return false;
	}

	file.clear();
	printf("[INF] %s sucessfully written\n", m_outputFilename);

	return true;
}

const Color32 *ImageConvert::getActiveColor(Color32 const *color) const
{
	return color;
}

bool ImageConvert::reduceColors(Color32Palette &palette)
{
	return PnnQuant::QuantizeImage(m_image, palette);
}

bool ImageConvert::remapColors(Color32Palette &palette)
{
	return PnnQuant::RemapImage(m_image, palette);
}

bool ImageConvert::loadAmiga(void)
{
	AmigaFile file;
	file.init();

	if (!file.load(m_image, m_colorInfo, m_inputFilename))
	{
		file.clear();
		return false;
	}

	file.clear();
	printf("[INF] %s sucessfully loaded\n", m_inputFilename);

	return true;
}

bool ImageConvert::writeAmiga(void)
{
	AmigaFile file;
	if (m_bitplanes == (uint16_t)invalid_t)
		m_bitplanes = 5;

	uint16_t colors = 1 << m_bitplanes;
	Color32Palette *palette = &m_colorInfo;
	Color32Palette amiga;

	if (m_colorInfo.size() > colors)
	{
		amiga.resize(colors);
		reduceColors(amiga);
		palette = &amiga;
	}

	if (!file.save(m_image, *palette, m_outputFilename, m_bitplanes, m_compress, m_interleave, m_saveColors))
		return false;

	printf("[INF] %s sucessfully written\n", m_outputFilename);

	return true;
}
