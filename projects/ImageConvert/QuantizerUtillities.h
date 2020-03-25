#ifndef _QUANTIZER_UTILLITIES_H
#define _QUANTIZER_UTILLITIES_H

#include "Color32Info.h"
#include "gfxImage.h"

typedef uint32_t (*DitherFn)(const Color32Palette &palette, const Color32 &color);

#define sqr(x) ((x) * (x))
#define INT_MAX 2147483647
#define BYTE_MAX 0xff
#define SHORT_MAX 32767

typedef DynamicArray<Color32> Color32Array;

bool ProcessImagePixels(RGBAImage &dest, const uint16_t *qPixels, bool hasSemiTransparency);
bool ProcessImagePixels(RGBAImage &dest, const Color32Palette &palette, const uint16_t *qPixels);
bool DitherRGB(RGBAImage &image, const Color32Palette &palette, DitherFn ditherFn, bool hasSemiTransparency);

inline int getARGBIndex(const Color32 &c, const bool hasSemiTransparency)
{
	if (hasSemiTransparency)
		return (c.rgb.a & 0xF0) << 8 | (c.rgb.r & 0xF0) << 4 | (c.rgb.g & 0xF0) | (c.rgb.b >> 4);

	return (c.rgb.r & 0xF8) << 8 | (c.rgb.g & 0xFC) << 3 | (c.rgb.b >> 3);
}

inline Color32Info MakeARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	Color32Info i;

	i.m_color.rgb.a = a;
	i.m_color.rgb.r = r;
	i.m_color.rgb.g = g;
	i.m_color.rgb.b = b;
	i.m_index = 0;

	return i;
}

#define _sqrt sqrt
/*#ifdef _WIN64
#define _sqrt sqrt
#endif // _WIN64

#ifdef _WIN32
inline double __fastcall _sqrt(double n)
{
	_asm fld qword ptr[esp + 4]
	_asm fsqrt
	_asm ret 8
}
#endif // _WIN32
*/
#endif // _QUANTIZER_UTILLITIES_H
