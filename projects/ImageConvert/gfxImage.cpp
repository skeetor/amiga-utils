
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <memory.h>

#include "gfxImage.h"

void RGBAImage::init(void)
{
	m_width = 0;
	m_height = 0;
	m_alpha = false;
	m_transparentColor = 0u;
	m_pixels.init();
}

void RGBAImage::clear(void)
{
	m_pixels.clear();

	init();
}

bool RGBAImage::createImage(uint16_t width, uint16_t height)
{
	m_width = width;
	m_height = height;

	m_pixels.resize(width * height);
	if (m_pixels.ptr() == nullptr)
		return false;

	return true;
}
