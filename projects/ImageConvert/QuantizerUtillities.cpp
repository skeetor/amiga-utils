/**
 * Fast pairwise nearest neighbor based algorithm for multilevel thresholding
 * Copyright (C) 2004-2016 Mark Tyler and Dmitry Groshev
 * Copyright (c) 2018-2019 Miller Cy Chan
 * error measure; time used is proportional to number of bins squared - WJ
 */

#include <stdio.h>

#include "utils/algorithms/DynamicArray.h"
#include "QuantizerUtillities.h"

static bool DitheringImage(const RGBAImage &image, const Color32Palette &palette, DitherFn ditherFn, bool hasSemiTransparency, void *qPixels)
{
	uint32_t width = image.m_width;
	uint32_t height = image.m_height;
	int16_t *row0, *row1;
	const int32_t DJ = 4;
	const int32_t DITHER_MAX = 20;
	const uint32_t err_len = (width + 2) * DJ;
	uint8_t clamp[DJ * 256] = { 0 };
	char limtb[512] = { 0 };
	char *lim = &limtb[256];
	DynamicArray<int16_t> erowerr(err_len, true);
	DynamicArray<int16_t> orowerr(err_len, true);
	DynamicArray<int32_t> lookup(65536, true);

	for (uint32_t i = 0; i < 256; i++)
	{
		clamp[i] = 0;
		clamp[i + 256] = static_cast<uint8_t>(i);
		clamp[i + 512] = BYTE_MAX;
		clamp[i + 768] = BYTE_MAX;

		limtb[i] = -DITHER_MAX;
		limtb[i + 256] = DITHER_MAX;
	}

	for (int32_t i = -DITHER_MAX; i <= DITHER_MAX; i++)
		limtb[i + 256] = i;

	uint32_t pixelIndex = 0;
	bool odd_scanline = false;

	for (uint32_t i = 0; i < height; i++)
	{
		int32_t dir;

		if (odd_scanline)
		{
			dir = -1;
			pixelIndex += (width - 1);
			row0 = &orowerr[DJ];
			row1 = &erowerr[width * DJ];
		}
		else
		{
			dir = 1;
			row0 = &erowerr[DJ];
			row1 = &orowerr[width * DJ];
		}

		row1[0] = row1[1] = row1[2] = row1[3] = 0;
		const Color32 *pixels = image.m_pixels;

		for (uint32_t j = 0; j < width; j++)
		{
			Color32 c = pixels[pixelIndex];
			int32_t k;

			int c1r = c.rgb.r;
			int c1g = c.rgb.g;
			int c1b = c.rgb.b;
			int c1a = c.rgb.a;

			c1r = clamp[((row0[0] + 0x1008) >> 4) + c1r];
			c1g = clamp[((row0[1] + 0x1008) >> 4) + c1g];
			c1b = clamp[((row0[2] + 0x1008) >> 4) + c1b];
			c1a = clamp[((row0[3] + 0x1008) >> 4) + c1a];

			Color32 c1;
			c1.setR(c1r);
			c1.setG(c1g);
			c1.setB(c1b);
			c1.setA(c1a);

			uint32_t offset = getARGBIndex(c1, hasSemiTransparency);
			if (!lookup[offset])
				lookup[offset] = ditherFn(palette, c1) + 1;

			Color32 c2;

			Color32 *qp = static_cast<Color32 *>(qPixels);
			c2 = palette[lookup[offset] - 1].m_color;
			qp[pixelIndex] = c2.value;

			c1r = lim[c1r - (int16_t)c2.rgb.r];
			c1g = lim[c1g - (int16_t)c2.rgb.g];
			c1b = lim[c1b - (int16_t)c2.rgb.b];
			c1a = lim[c1a - (int16_t)c2.rgb.a];

			k = c1r * 2;
			row1[0 - DJ] = c1r;
			row1[0 + DJ] += (c1r += k);
			row1[0] += (c1r += k);
			row0[0 + DJ] += (c1r += k);

			k = c1g * 2;
			row1[1 - DJ] = c1g;
			row1[1 + DJ] += (c1g += k);
			row1[1] += (c1g += k);
			row0[1 + DJ] += (c1g += k);

			k = c1b * 2;
			row1[2 - DJ] = c1b;
			row1[2 + DJ] += (c1b += k);
			row1[2] += (c1b += k);
			row0[2 + DJ] += (c1b += k);

			k = c1a * 2;
			row1[3 - DJ] = c1a;
			row1[3 + DJ] += (c1a += k);
			row1[3] += (c1a += k);
			row0[3 + DJ] += (c1a += k);

			row0 += DJ;
			row1 -= DJ;
			pixelIndex += dir;
		}

		if ((i % 2) == 1)
			pixelIndex += (width + 1);

		odd_scanline = !odd_scanline;
	}

	return true;
}

bool DitherRGB(RGBAImage &image, const Color32Palette &palette, DitherFn ditherFn, bool hasSemiTransparency)
{
	return DitheringImage(image, palette, ditherFn, hasSemiTransparency, image.m_pixels);
}
