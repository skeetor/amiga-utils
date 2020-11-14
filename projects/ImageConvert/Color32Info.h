#ifndef _COLORINFO_H
#define _COLORINFO_H

#include <utils/utils_def.h>
#include "gfxImage.h"
#include "utils/algorithms/SortedArray.h"

class Color32Info
{
public:
	REGPARAM(0) Color32Info(void)
	{
		init();
	}

	REGPARAM(1) Color32Info(Color32 const &color)
	{
		operator=(color);
	}

	REGPARAM(1) Color32Info(uint32_t value)
	{
		init();
		m_color.value = value;
	}

	REGPARAM(0) ~Color32Info(void)
	{
	}

	REGPARAM(1) Color32Info &operator=(const uint32_t value)
	{
		m_color = value;

		return *this;
	}

	REGPARAM(1) Color32Info &operator=(Color32 const &color)
	{
		if (&color == &m_color)
			return *this;

		init();
		m_color = color;

		return *this;
	}

	// Default comparator using the color value
	REGPARAM(1) bool operator<(const Color32Info &other) const
	{
		return m_color.value < other.m_color.value;
	}

protected:
	REGPARAM(0) void init(void);

public:
	Color32 m_color;
	uint32_t m_index;
};

class Color32Palette
: public SortedArray<Color32Info>
{
public:
	//typedef RawIterator<Color32Info> iterator;
	//typedef RawIterator<const Color32Info> const_iterator;

public:
	REGPARAM(0) Color32Palette(void)
	{
		init();
	}

	REGPARAM(1) Color32Palette(size_t reserved)
	{
		init();
		reserve(reserved);
	}

	REGPARAM(0) ~Color32Palette(void) override
	{
		clear();
	}

	REGPARAM(0) void clear(void);

	//REGPARAM(0) iterator begin(void) const { return iterator(&ptr()[0]); }
	//REGPARAM(0) iterator end(void) const { return iterator(&ptr()[size()]); }

/*	REGPARAM(0) const_iterator cbegin(void) { return const_iterator(&this[0]); }
	REGPARAM(0) const_iterator cend(void) { return const_iterator(&this[size()]); }
*/
	/**
	 * Sort by usage counter
	 */
	REGPARAM(0) void sortByIndex(bool ascending = true);

	/**
	 * Sort by color value
	 */
	REGPARAM(0) void sortByColor(void);

	REGPARAM(2) size_t nearestColorMatch(const Color32& color, size_t maxIndex) const;

	/**
	 * Write the color usage to the specified statistics file.
	 * It can be sorted either by color or by usage counter.
	 */
	REGPARAM(2) bool writeStatistics(const char *filename, bool byColor);

	/**
	 * Write the palette file for the current image, if a filename is specified.
	 * Returns true if the file was sucessfully written or not required.
	 * Returns false, if the file couldn't be written.
	 */
	REGPARAM(2) bool writePalettefile(const char *filename, const char *header = nullptr);

protected:
	REGPARAM(0) void init(void);
};

// For debugging
void dumpColorInfo(Color32Palette &colorInfo, const char *filename, bool byColor);

#endif // _COLORINFO_H
