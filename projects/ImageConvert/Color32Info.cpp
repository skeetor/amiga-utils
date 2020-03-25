#include "utils/algorithms/DynamicArray.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <memory.h>

#include "Color32Info.h"

// Just for debugging
void dumpColorInfo(Color32Palette &colorInfo, const char *filename, bool byColor)
{
	if (byColor)
		colorInfo.sortByColor();
	else
		colorInfo.sortByIndex(false);

	FILE *f = fopen(filename, "wb");
	if (f)
	{
		for (uint32_t i = 0; i < colorInfo.size(); i++)
		{
			Color32Info *info = &colorInfo[i];

			Color32 *img = &info->m_color;

			fprintf(f, "%u: %03u:%03u:%03u %08lX : Count: %lu\n"
				, i
				, img->rgb.r
				, img->rgb.g
				, img->rgb.b
				, img->value
				, info->m_index
			);
		}
		fclose(f);
	}
}

static bool cmpByIndexDescending(const Color32Info &e1, const Color32Info &e2)
{
	if (e1.m_index < e2.m_index)
		return true;

	if (e1.m_index == e2.m_index)
	{
		// Reverse colors so that they are ordered by their value
		if (e1.m_color.value > e2.m_color.value)
			return true;
	}

	return false;
}

static bool cmpByIndexAscending(const Color32Info &e1, const Color32Info &e2)
{
	if (e1.m_index > e2.m_index)
		return true;

	if (e1.m_index == e2.m_index)
	{
		if (e1.m_color.value > e2.m_color.value)
			return true;
	}

	return false;
}

// Sorting is reversed, because we want the highest count first.
static int CDECL sortByIndexDescending(const Color32Info *e1, const Color32Info *e2)
{
	if (cmpByIndexDescending(*e1, *e2))
		return 1;

	if (cmpByIndexDescending(*e2, *e1))
		return -1;

	return 0;
}

static int CDECL sortByIndexAscending(const Color32Info *e1, const Color32Info *e2)
{
	if (cmpByIndexAscending(*e1, *e2))
		return 1;

	if (cmpByIndexAscending(*e2, *e1))
		return -1;

	return 0;
}

REGPARAM(0) void Color32Info::init(void)
{
	m_color.value = 0;
	m_index = 0;
}

REGPARAM(0) void Color32Palette::init(void)
{
	SortedArray<Color32Info>::init();
}

REGPARAM(0) void Color32Palette::clear(void)
{
	init();
}

REGPARAM(0) void Color32Palette::sortByIndex(bool ascending)
{
	if (ascending)
		setComparator((comparisor)sortByIndexAscending);
	else
		setComparator((comparisor)sortByIndexDescending);
}

REGPARAM(0) void Color32Palette::sortByColor(void)
{
	setComparator();
}

REGPARAM(2) bool Color32Palette::writeStatistics(const char *filename, bool byColor)
{
	if (!filename)
		return false;

	FILE *cf = fopen(filename, "wb");
	if (cf)
	{
		fprintf(cf, "# color usage for: %s\n", filename);

		if (byColor)
			sortByColor();
		else
			sortByIndex(false);

		Color32Info *info = ptr();
		size_t sz = size();
		for (size_t i = 0; i < sz; i++)
		{
			fprintf(cf, "%zu : %08lX : %lu\n", i, info->m_color.value, info->m_index);
			info++;
		}
		fprintf(cf, "# Total colors used: %zu\n", sz);

		fclose(cf);
	}
	else
		return false;

	return true;
}

bool Color32Palette::writePalettefile(const char *filename, const char *header)
{
	if (filename)
	{
		FILE *out = fopen(filename, "wb");
		if (!out)
			return false;

		if (header)
			fprintf(out, "%s\n", header);

		Color32Info *info = ptr();
		for (uint32_t i = 0; i < size(); i++)
		{
			fprintf(out, "%8u:0x%08lX\n", i, info->m_color.value);
			info++;
		}
		fclose(out);
	}

	return true;
}

size_t Color32Palette::nearestColorMatch(const Color32& color, size_t maxIndex) const
{
	size_t index = 0;
	uint16_t minDistance = (uint16_t)invalid_t;

	const Color32Info *col = ptr();

	for (size_t i = 0; i < maxIndex; i++)
	{
		uint32_t dr = abs((int)(col->m_color.rgb.r - color.rgb.r));
		uint32_t dg = abs((int)(col->m_color.rgb.g - color.rgb.g));
		uint32_t db = abs((int)(col->m_color.rgb.b - color.rgb.b));

		uint32_t distance = dr*dr + dg*dg + db*db;

		// It can not get better than this. :)
		if (!distance)
			return i;

		if (distance < minDistance)
		{
			minDistance = distance;
			index = i;
		}

		col++;
	}

	return index;
}
