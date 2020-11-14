#ifndef _GFXIMAGE_H
#define _GFXIMAGE_H

#include <stdint.h>

#include "utils/algorithms/DynamicArray.h"

typedef struct _RGB8
{
#ifdef _MSC_VER
	// Reverse bytes for intel
	uint8_t b : 2;
	uint8_t g : 3;
	uint8_t r : 3;
#else
	uint8_t r : 3;
	uint8_t g : 3;
	uint8_t b : 2;
#endif
} RGB8;
static_assert(sizeof(RGB8) * 8 == 8, "RGB structure exceeds 8 bit");

typedef union _Color8
{
	RGB8 rgb;
	uint8_t value;
} Color8;

typedef struct _RGB16
{
#ifdef AMIGA_OS
	uint16_t a : 1;
	uint16_t r : 5;
	uint16_t g : 5;
	uint16_t b : 5;
#else
	// Reverse bytes for intel
	uint16_t b : 5;
	uint16_t g : 5;
	uint16_t r : 5;
	uint16_t a : 1;
#endif
} RGB16;
static_assert(sizeof(RGB16)*8 == 16, "RGB structure exceeds 16 bit");

typedef union _Color16
{
	RGB16 rgb;
	uint16_t value;
} Color16;

typedef struct _RGB32
{
#ifdef AMIGA_OS
	uint32_t a : 8;
	uint32_t r : 8;
	uint32_t g : 8;
	uint32_t b : 8;
#else
	// Reverse bytes for intel
	uint32_t b : 8;
	uint32_t g : 8;
	uint32_t r : 8;
	uint32_t a : 8;
#endif
} RGB32;
static_assert(sizeof(RGB32) * 8 == 32, "RGB structure exceeds 32 bit");

typedef union _Color32 Color32;
typedef union _Color32
{
	uint32_t value;
	RGB32 rgb;

	bool operator==(const Color32 &other) const
	{
		return other.value == value;
	}

	bool operator!=(const Color32 &other) const
	{
		return other.value != value;
	}

	Color32& operator=(uint32_t val)
	{
		value = val;
		return *this;
	}

	uint32_t getR(void) const
	{
		return rgb.r;
	}

	void setR(uint32_t v)
	{
		rgb.r = v;
	}

	uint32_t getG(void) const
	{
		return rgb.g;
	}

	void setG(uint32_t v)
	{
		rgb.g = v;
	}

	uint32_t getB(void) const
	{
		return rgb.b;
	}

	void setB(uint32_t v)
	{
		rgb.b = v;
	}

	uint32_t getA(void) const
	{
		return rgb.a;
	}

	void setA(uint32_t v)
	{
		rgb.a = v;
	}

} Color32;

typedef struct _RGBAImage
{
	uint32_t m_width;
	uint32_t m_height;
	DynamicArray<Color32> m_pixels;		// Line by line
	bool m_alpha;
	Color32 m_transparentColor;

	void init(void);

	/**
	 * Resets the structures and releases allocated memory.
	 */
	void clear(void);

	/**
	 * Allocates the buffer for the image.
	 * If an error occured, false is returned and the memory
	 * is released.
	 */
	bool createImage(uint16_t width, uint16_t height);
} RGBAImage;

#endif // _GFXIMAGE_H
