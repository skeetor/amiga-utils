
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <memory.h>

#include <utils/utils_def.h>

#include "ImageConvert.h"

/**
* Convert png to Amiga bitplanes
*     -i tmp/green.png -o amiga tmp/green_amiga_5d.abpl
*     -i tmp/green.png -o amiga 5 tmp/green_amiga_5.abpl
*     -i tmp/green.png -o amiga 3 tmp/green_amiga_3.abpl
*
* Convert Amiga Raw Bitplane format to png
*     -i amiga tmp/green_amiga_5.abpl -o tmp/green_amiga_5.png
*     -i amiga tmp/green_amiga_3.abpl -o tmp/green_amiga_3.png
*
* Convert png palette png
*     -i tmp/green.png -o png tmp/green_png_p.png
*
* Convert png to RGB png
*     -i tmp/green.png -o png-rgb tmp/green_png_rgb.png
*
* Convert png to RGB png reducing colors
*     --colors 256 -i tmp/green.png -o png-rgb tmp/green_png_rgb_256.png
*/

#ifdef AMIGA_OS
#include "utils/dos/dos.h"
#include "utils/dos/stdio.h"
#else
void OpenDOSLibrary(void);
void CloseDOSLibrary(void);
#endif

static void help(void)
{
	const char *s =
R"(ImageConvert (c) by Gerhard W. Gruber - source at https://github.com/skeetor/amiga-utils
This file may be freely distributed.

Arguments:
-h|--help                print this help
-i [type parameters] imagefile
                         input file. If type is not specified 'png' is default. Mandatory.
-o [type parameters] outputfile
                         output file (only needed if the image should be modified).
-s [c/u] statisticsfile  Writes a file with the colors and how often they are used.
                              c = sort by color (default)
                              u = sort by usagecount
-d palettefile           dump palette of input file (this file can be used as input to -p).
-p palettefile           palettefile overriding the colors of the graphics. If the image has colors
                         not in the palette the closest matching color will be used. If the --colors
                         parameter is also given it will be applied first, before the palette is applied.
--colors [N]             Reduce colors to N (2-65535). Note that some outputformats
                         may require additional reductions which are applied automatically.
                         The reduction of colors will never yield more then tha specified number
                         of colors, but, due to the algorithm, it might be less.
--compress               Compress output with gzip format (only applicable for certain formats).
--interleave             Write bitplane data interleaved  (only applicable for certain formats).
--no-colors              Depending on the output format no colors are stored in the image.
                         Currently this only is the case for amiga format.

[type parameters]
If the type is specified for the input, there are no additional parameters needed.

png[-rgb]                Write as PNG file. Note that PNG palette supports only
                         a maximum of 256 colors, so they are reduced if needed.
                         If the inputfile has an alpha channel it is preserved. Palette
                         is the default.
amiga <N>                N specifies the number of bitplanes to be written (1-8). Interleave
                         writes the color data directly after each other. Default is to
                         write each bitplane seperately one after the other. If the number
                         is ommited it is determined by the current number of colors. In this
                         case the image is by default reduced to 5 bitplanes if the number of
                         colors is higher than 32.
amiga-bpl <N>            Same as 'amiga' only the width is extended to the next multiple of 16
                         if it is not already a multiple of 16. This way the image can be
                         directly shown on screen.
)";

	puts(s);
}

static int parseType(bool input, ImageConvert &converter, int i, int argc, char *argv[])
{
	const char *arg = argv[i];

	if (strcmp(arg, "png-rgb") == 0)
	{
		if (input)
			converter.imageLoader = &ImageConvert::loadPng;
		else
			converter.imageWriter = &ImageConvert::writePngRGB;

		return i+1;
	}

	if (strcmp(arg, "amiga") == 0)
	{
		i++;

		if (input)
			converter.imageLoader = &ImageConvert::loadAmiga;
		else
		{
			if (i < argc)
			{
				if (argv[i][1] == 0)
				{
					if (argv[i][0] >= '0' && argv[i][0] <= '8')
					{
						converter.m_bitplanes = (uint32_t)argv[i][0] - '0';
						i++;
					}
					else
					{
						printf("[ERR] Invalid parameter for amiga bitplanes\n");
						return -1;
					}
				}
			}

			converter.imageWriter = &ImageConvert::writeAmiga;
		}

		return i;
	}

	// Default is PNG
	if (strcmp(arg, "png") == 0)
		i++;

	if (input)
		converter.imageLoader = &ImageConvert::loadPng;
	else
		converter.imageWriter = &ImageConvert::writePngPalette;

	return i;
}

static bool parseArguments(ImageConvert &converter, int argc, char *argv[])
{
	int i = 1;

	while (i < argc)
	{
		const char *arg = argv[i++];

		if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
		{
			help();
			return false;
		}

		if (strcmp(arg, "-i") == 0)
		{
			i = parseType(true, converter, i, argc, argv);
			if (i == -1)
				return false;

			if (i >= argc)
			{
				printf("[ERR] Missing input filename (-if <filename>)\n");
				return false;
			}

			converter.m_inputFilename = argv[i++];
			continue;
		}

		if (strcmp(arg, "-o") == 0)
		{
			i = parseType(false, converter, i, argc, argv);
			if (i == -1)
				return false;

			if (i >= argc)
			{
				printf("[ERR] Missing output filename (-of <filename>)\n");
				return false;
			}

			converter.m_outputFilename = argv[i++];
			continue;
		}

		if (strcmp(arg, "-d") == 0)
		{
			if (i >= argc)
			{
				printf("[ERR] Missing filename for palette convertfile (-d <filename>)\n");
				return false;
			}

			converter.m_paletteDumpFilename = argv[i++];
			continue;
		}

		if (strcmp(arg, "-p") == 0)
		{
			if (i >= argc)
			{
				printf("[ERR] Missing filename for target palette (-p <filename>)\n");
				return false;
			}

			converter.m_targetPaletteFilename = argv[i++];
			continue;
		}

		if (strcmp(arg, "-s") == 0)
		{
			if (i >= argc)
			{
				printf("[ERR] Missing filename for color statistics (-s <filename>)\n");
				return false;
			}

			converter.m_colorStatsFilename = argv[i++];
			if (i >= argc || argv[i][0] == '-')
				continue;

			// Default is color, so we only need to change it if it is different
			if (*converter.m_colorStatsFilename == 'u')
			{
				converter.m_sortByColor = false;
				converter.m_colorStatsFilename = argv[i++];
			}

			continue;
		}

		if (strcmp(arg, "--colors") == 0)
		{
			if (i >= argc)
			{
				printf("[ERR] Missing value for color reduction\n");
				return false;
			}

			converter.m_reduceColors = (uint32_t)atol(argv[i++]);
			continue;
		}

		if (strcmp(arg, "--compress") == 0)
		{
			converter.m_compress = true;
			continue;
		}

		if (strcmp(arg, "--interleave") == 0)
		{
			converter.m_interleave = true;
			continue;
		}
	}

	if (!converter.m_inputFilename)
	{
		puts("[ERR] No input filenname specified!\n");
		return false;
	}

	if (!converter.m_outputFilename && !converter.m_paletteDumpFilename && !converter.m_colorStatsFilename)
	{
		puts("[ERR] No operation specified!\n");
		return false;
	}

	return true;
}

STDARGS int main(int argc, char *argv[])
{
	int rc = 0;

	OpenDOSLibrary();

	ImageConvert converter;

	if (argc <= 1)
	{
		help();
		goto Quit;
	}

	if (!parseArguments(converter, argc, argv))
	{
		rc = 1;
		goto Quit;
	}

	if (!converter.loadImage())
	{
		rc = 1;
		goto Quit;
	}

	// Write the stats from the original file
	if (converter.m_colorStatsFilename)
	{
		printf("[INF] Writing color usage to %s\n", converter.m_colorStatsFilename);

		if (!converter.m_colorInfo.writeStatistics(converter.m_colorStatsFilename, converter.m_sortByColor))
			printf("[ERR] Unable to write color statistics to %s\n", converter.m_colorStatsFilename);
	}

	// Reduce the original colors to the specified number
	if (converter.m_reduceColors != invalid_t)
	{
		printf("[INF] Reducing colors from %zu to %lu\n", converter.m_colorInfo.size(), converter.m_reduceColors);

		converter.m_colorInfo.resize(converter.m_reduceColors);
		converter.reduceColors(converter.m_colorInfo);
	}

	// Dump the current colors to a palettefile
	if (converter.m_paletteDumpFilename)
	{
		char buffer[302] = { 0 };
		snprintf(buffer, sizeof(buffer) - 1, "# Autogenerated by ImageConvert. Format is RGB 24 bit\n# Imagename: %s\n#\n", converter.m_inputFilename);
		if (!converter.m_colorInfo.writePalettefile(converter.m_paletteDumpFilename, buffer))
		{
			printf("[ERR] Error writing source image palette to %s\n", converter.m_paletteDumpFilename);
			rc = 1;
			goto Quit;
		}

		printf("[INF] Source image palette written to %s\n", converter.m_paletteDumpFilename);
	}

	if (converter.m_targetPaletteFilename)
	{
		if (!converter.loadPaletteFile(converter.m_targetPaletteFilename, converter.m_targetPalette))
		{
			printf("[ERR] Error reading palette file %s\n", converter.m_targetPaletteFilename);
			rc = 1;
			goto Quit;
		}

		converter.remapColors(converter.m_targetPalette);
	}

	if (converter.m_outputFilename)
	{
		if (!converter.writeImage())
		{
			printf("[ERR] Error writing output image to %s\n", converter.m_outputFilename);
			rc = 1;
			goto Quit;
		}
	}

Quit:
	CloseDOSLibrary();

	return rc;
}
