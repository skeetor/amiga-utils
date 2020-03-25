
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <memory.h>

#include "PngFile.h"

using namespace std;

namespace
{
	static const int palette_max_size = 256;

	/**
	 * Callback function which will read the data from our source, which is called by
	 * libPNG whenever it needs to read additional data.
	 * In this case it is a file, but of course it can be easily converted to read from
	 * memory as well.
	 */
	extern "C" PNG_CALLTYPE void PNGIOData(png_structp pngHandle, png_bytep buffer, png_size_t bytesToTransfer)
	{
		png_voidp io_ptr = png_get_io_ptr(pngHandle);
		if (io_ptr == NULL)
		{
			png_error(pngHandle, "[ERR] Invalid io buffer pointer in PNGIOData()");

			return;
		}

		PngFile *file = (PngFile *)io_ptr;
		size_t bytesTransfered;
		if (file->m_readMode)
			bytesTransfered = fread(buffer, 1, bytesToTransfer, file->m_file);
		else
			bytesTransfered = fwrite(buffer, 1, bytesToTransfer, file->m_file);

		if (bytesTransfered != bytesToTransfer)
		{
			const char *msg;
			if (file->m_readMode)
				msg = "reading";
			else
				msg = "writing";

			char buffer[302] = { 0 };
			snprintf(buffer, sizeof(buffer) - 1, "[ERR] Error %s file", msg);
			png_error(pngHandle, buffer);
		}
	}

	extern "C" PNG_CALLTYPE void PNGErrorHandler(png_structp pngHandle, const char *msg)
	{
		png_voidp io_ptr = png_get_io_ptr(pngHandle);
		if (io_ptr == NULL)
		{
			png_error(pngHandle, "[ERR] Invalid io buffer pointer in PNGErrorHandler()");

			return;
		}

		PngFile *file = (PngFile *)io_ptr;

		printf("[ERR] PNG error: %s", msg);

		// TODO: Fix memory leak in case of error handling.
		// This will cause memory leaks on Amiga, because PNG will not return after the
		// error handle is called, so we would need to record all memory usage in some
		// different object.
		file->clear();
	}

	extern "C" PNG_CALLTYPE void PNGWarningHandler(png_structp pngHandle, const char *msg)
	{
		printf("[WRN] PNG error: %s", msg);
	}
} // namespace

void PngFile::init(void)
{
	m_file = nullptr;
	m_handle = nullptr;
	m_info = nullptr;
	m_palette = nullptr;
	m_readMode = false;
}

void PngFile::clear(void)
{
	if (m_palette)
		png_free(m_handle, m_palette);

	if(m_readMode)
		png_destroy_read_struct(&m_handle, &m_info, NULL);
	else
		png_destroy_write_struct(&m_handle, &m_info);

	if (m_file)
		fclose(m_file);

	init();
}

bool PngFile::parseImage(RGBAImage &image, Color32Palette &colors, bool alpha, ColorConverter colorConverter)
{
	size_t bytesPerRow = png_get_rowbytes(m_handle, m_info);
	size_t sz = bytesPerRow;
	uint8_t *imgRow = (uint8_t *)malloc(sz);
	if (!imgRow)
	{
		printf("[ERR] Unable to allocate %zu bytes for image data\n", sz);
		return false;
	}

	Color32 *img = image.m_pixels;
	const uint8_t *e = &imgRow[bytesPerRow];
	colors.clear();

	// read single row at a time
	for (uint32_t rowIdx = 0; rowIdx < image.m_height; ++rowIdx)
	{
		uint8_t *p = imgRow;
		png_read_row(m_handle, (png_bytep)p, NULL);

		while(p < e)
		{
			p = colorConverter(p, *img, alpha);

			if (!p)
				return false;

			Color32Info c;
			c.m_color.value = img->value;
			size_t index;
			if(!colors.add(c, &index))
			{
				if (index == invalid_t)
				{
					printf("[ERR] Unable to allocate color information\n");
					free(imgRow);
					return false;
				}
			}

			colors[index].m_index++;

			img++;
		}
	}

	free(imgRow);

	return true;
}

bool PngFile::open(const char *filename, bool bRead)
{
	m_readMode = bRead;

	const char *mode;
	if (bRead)
	{
		m_handle = png_create_read_struct(PNG_LIBPNG_VER_STRING, this, PNGErrorHandler, PNGWarningHandler);
		mode = "rb";
	}
	else
	{
		m_handle = png_create_write_struct(PNG_LIBPNG_VER_STRING, this, PNGErrorHandler, PNGWarningHandler);
		mode = "wb";
	}

	if (m_handle == NULL)
		return false;

	m_info = png_create_info_struct(m_handle);
	if (m_info == NULL)
		return false;

	if ((m_file = fopen(filename, mode)) == nullptr)
		return false;

	return true;
}

bool PngFile::parseRGB(RGBAImage &image, Color32Palette &colors, bool alpha)
{
	return parseImage(image, colors, alpha,
		[&](uint8_t *fileData, Color32 &img, bool alpha)
		{
			img.rgb.r = *fileData++;
			img.rgb.g = *fileData++;
			img.rgb.b = *fileData++;

			if (alpha)
			{
				img.rgb.a = *fileData++;
				if (img.rgb.a == 0)
					image.m_transparentColor.value = img.value;
			}
			else
				img.rgb.a = static_cast<uint8_t>(invalid_t);

			return fileData;
		}
	);
}

bool PngFile::parsePalette(RGBAImage &image, Color32Palette &colors)
{
	png_colorp palette;
	uint32_t num_palette = 0;

	png_uint_32 rc = png_get_PLTE(m_handle, m_info, &palette, reinterpret_cast<int *>(&num_palette));
	if (!rc)
		return false;

	return parseImage(image, colors, false,
		[&](uint8_t *fileData, Color32 &img, bool alpha)
		{
			uint32_t index = *fileData++;

			if (index > num_palette)
			{
				fileData = nullptr;
				return fileData;
			}

			img.rgb.r = palette[index].red;
			img.rgb.g = palette[index].green;
			img.rgb.b = palette[index].blue;
			img.rgb.a = 0xff;

			return fileData;
		}
	);
}

bool PngFile::load(RGBAImage &image, Color32Palette &colors, const char *filename)
{
	printf("[INF] Loading %s\n", filename);

	if (!open(filename, true))
	{
		printf("[ERR] Error opening PNG file for reading: %s\n", filename);
		return false;
	}

	png_set_read_fn(m_handle, this, PNGIOData);

	png_read_info(m_handle, m_info);

	uint16_t width = png_get_image_width(m_handle, m_info);
	uint16_t height = png_get_image_height(m_handle, m_info);

	if (!image.createImage(width, height))
		return false;

	int colorType = png_get_color_type(m_handle, m_info);
	int bitDepth = png_get_bit_depth(m_handle, m_info);
	int passes = png_set_interlace_handling(m_handle);

	const char *ctype;
	bool rc = false;

	switch (colorType)
	{
		case PNG_COLOR_TYPE_RGB:
		{
			rc = parseRGB(image, colors, false);
			ctype = "PNG_COLOR_TYPE_RGB";
			image.m_alpha = false;
		}
		break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			rc = parseRGB(image, colors, true);
			ctype = "PNG_COLOR_TYPE_RGB_ALPHA";
			image.m_alpha = true;
		}
		break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			rc = parseRGB(image, colors, true);
			ctype = "PNG_COLOR_TYPE_GRAY_ALPHA";
			image.m_alpha = true;
		}
		break;

		case PNG_COLOR_TYPE_GRAY:
		{
			rc = parseRGB(image, colors, false);
			ctype = "PNG_COLOR_TYPE_GRAY";
		}
		break;

		case PNG_COLOR_TYPE_PALETTE:
		{
			rc = parsePalette(image, colors);
			ctype = "PNG_COLOR_TYPE_PALETTE";
			rc = true;
		}
		break;

		default:
			ctype = "UNKNOWN";
	}

	if (rc)
		printf("[INF] W: %lu H: %lu ColType: %s Depth: %u Passes: %u\n", width, height, ctype, bitDepth, passes);
	else
		printf("[ERR] Error loading PNG data of type: %s\n", ctype);

	clear();

	return rc;
}

bool PngFile::initWrite(RGBAImage const &image, const char *filename, int imgType, int depth)
{
	// No output filename is ok.
	if (!filename)
		return true;

	if (!open(filename, false))
	{
		printf("[ERR] Error opening PNG file for writing: %s\n", filename);
		return false;
	}

	//png_set_write_fn(m_handle, this, PNGIOData, PNGWriteFlush);
	png_set_write_fn(m_handle, this, PNGIOData, NULL);

	png_set_IHDR(
		m_handle,
		m_info,
		image.m_width, image.m_height,
		depth,
		imgType,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	return true;
}

/*int PngFile::getColorDepth(uint32_t &colors) const
{
	if (colors > palette_max_size)
		colors = palette_max_size;

	if (colors > 16)
		return 8;

	if (colors > 4)
		return 4;

	if (colors > 2)
		return 2;

	return 1;
}*/

bool PngFile::writePaletteColors(Color32Palette &colors)
{
	if (colors.size() <= 1)
		return false;

	m_palette = (png_colorp)png_malloc(m_handle, palette_max_size * sizeof(png_color));
	if (m_palette == NULL)
		return false;

	memset(m_palette, 0, palette_max_size * sizeof(png_color));

	colors.sortByIndex();

	png_colorp p = m_palette;
	const Color32Info *c = colors.ptr();
	for (uint32_t i = 0; i < colors.size(); i++)
	{
		p->red = c->m_color.rgb.r;
		p->green = c->m_color.rgb.g;
		p->blue = c->m_color.rgb.b;

		p++;
		c++;
	}

	// Commit the palette
	png_set_PLTE(m_handle, m_info, m_palette, static_cast<int>(colors.size()));
	png_write_info(m_handle, m_info);

	return true;
}

bool PngFile::writePaletteRows(RGBAImage const &image, Color32Palette &colors)
{
	uint8_t *imgRow = (uint8_t *)malloc(image.m_width * sizeof(uint8_t));
	if (!imgRow)
	{
		printf("[ERR] Unable to allocate memory for palette rows\n");
		return false;
	}

	colors.sortByColor();
	const Color32 *img = image.m_pixels;
	for (uint32_t y = 0; y < image.m_height; y++)
	{
		uint8_t *p = imgRow;

		for (uint32_t x = 0; x < image.m_width; x++)
		{
			size_t index = colors.find(*img);
			*p++ = (uint8_t)colors[index].m_index;
			img++;
		}

		png_write_row(m_handle, imgRow);
	}
	png_write_end(m_handle, NULL);

	free(imgRow);

	return true;
}

bool PngFile::savePaletteFormat(RGBAImage const &image, Color32Palette &colors, const char *filename)
{
	int depth = 8;	// TODO: Should be able to use a userdefined value

	if (colors.size() > palette_max_size)
		return false;

	if (!initWrite(image, filename, PNG_COLOR_TYPE_PALETTE, depth))
		return false;

	bool rc;
	if ((rc = writePaletteColors(colors)))
		rc = writePaletteRows(image, colors);

	clear();

	return rc;
}

bool PngFile::saveRGBFormat(RGBAImage const &image, Color32Palette &colors, const char *filename)
{
	uint32_t sz;
	int imgType = (image.m_alpha) ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB;

	if (image.m_alpha)
	{
		imgType = PNG_COLOR_TYPE_RGBA;
		sz = image.m_width * 4;
	}
	else
	{
		imgType = PNG_COLOR_TYPE_RGB;
		sz = image.m_width * 3;
	}

	if (!initWrite(image, filename, imgType, 8))
	{
		clear();
		return false;
	}

	uint8_t *imgRow = (uint8_t *)malloc(sz * sizeof(uint8_t));
	if (!imgRow)
	{
		clear();
		return false;
	}

	colors.sortByColor();

	png_write_info(m_handle, m_info);
	uint8_t *e = &imgRow[sz];
	const Color32 *img = image.m_pixels;

	for (uint32_t i = 0; i < image.m_height; i++)
	{
		uint8_t *p = imgRow;
		while (p < e)
		{
			Color32 const &col = *img;

			*p++ = col.rgb.r;
			*p++ = col.rgb.g;
			*p++ = col.rgb.b;

			if (image.m_alpha)
				*p++ = col.rgb.a;

			img++;
		}

		png_write_row(m_handle, imgRow);
	}

	png_write_end(m_handle, NULL);

	clear();
	free(imgRow);

	return true;
}
