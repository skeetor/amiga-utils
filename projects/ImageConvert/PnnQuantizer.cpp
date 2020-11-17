/**
 * Fast pairwise nearest neighbor based algorithm for multilevel thresholding
 * Copyright (C) 2004-2016 Mark Tyler and Dmitry Groshev
 * Copyright (c) 2018-2019 Miller Cy Chan
 * error measure; time used is proportional to number of bins squared - WJ
 */

#include <math.h>

#include "QuantizerUtillities.h"

#define POW2(x) ((x)*(x))
#define EPSILON 0.0000001

// ********************************************************************
namespace PnnQuant
{
	template<typename T>
	void swap(T &a, T &b)
	{
		T v = a;
		a = b;
		b = v;
	}

	class ColorMatch
	{
	public:
		ColorMatch(const Color32 &c, uint32_t index)
		: m_index(index)
		{
			m_color = c.value;

			y = 0.299*(double)c.rgb.r + 0.587*(double)c.rgb.g + 0.114*(double)c.rgb.b;
			u = -0.147*(double)c.rgb.r - 0.289*(double)c.rgb.g + 0.436*(double)c.rgb.b;
			v = 0.615*(double)c.rgb.r - 0.515*(double)c.rgb.g - 0.100*(double)c.rgb.b;

			m_length = sqrt(POW2(y) + POW2(u) + POW2(v));
		}

		// Default comparator using the color value
		bool operator<(const ColorMatch &other) const
		{
			return m_color.value < other.m_color.value;
		}

		// Default comparator using the color value
		bool operator<(const Color32 &other) const
		{
			return m_color.value < other.value;
		}

		double y, u, v;
		uint32_t m_index;
		Color32 m_color;
		double m_length;
	};

	// https://www.mathebibel.de/winkel-zwischen-zwei-vektoren
/*
	uint32_t angleOfVectors(const Color32 &u, const Color32 &v)
	{
		double sProd = u.rgb.r*v.rgb.r + u.rgb.g*v.rgb.g + u.rgb.b*v.rgb.b;
		double lu = sqrt(POW2(u.rgb.r) + POW2(u.rgb.g) + POW2(u.rgb.b));
		double lv = sqrt(POW2(v.rgb.r) + POW2(v.rgb.g) + POW2(v.rgb.b));
		double angle = acos(sProd / (lu*lv));

		return static_cast<uint32_t>(angle * 10000);
	}

	uint32_t angleOfVectorsA(const Color32 &u, const Color32 &v)
	{
		double sProd = u.rgb.a*v.rgb.a + u.rgb.r*v.rgb.r + u.rgb.g*v.rgb.g + u.rgb.b*v.rgb.b;
		double lu = sqrt(POW2(u.rgb.a) + POW2(u.rgb.r) + POW2(u.rgb.g) + POW2(u.rgb.b));
		double lv = sqrt(POW2(v.rgb.a) + sqrt(POW2(v.rgb.r) + POW2(v.rgb.g) + POW2(v.rgb.b));
		double angle = acos(sProd / (lu*lv));

		return static_cast<uint32_t>(angle * 10000);
	}*/

	static bool hasSemiTransparency = false;
	static Color32 transparentColor = { 0x00FFFFFF };

	struct pnnbin
	{
		double ac = 0, rc = 0, gc = 0, bc = 0, err = 0;
		int cnt = 0;
		int nn = 0, fw = 0, bk = 0, tm = 0, mtm = 0;
	};

	static void findNearestNeighbor(pnnbin* bins, int idx)
	{
		int nn = 0;
		double err = 1e100;

		pnnbin & bin1 = bins[idx];
		int n1 = bin1.cnt;
		double wa = bin1.ac;
		double wr = bin1.rc;
		double wg = bin1.gc;
		double wb = bin1.bc;

		for (int i = bin1.fw; i; i = bins[i].fw)
		{
			double nerr = sqr(bins[i].rc - wr) + sqr(bins[i].gc - wg) + sqr(bins[i].bc - wb);
			if(hasSemiTransparency)
				nerr += sqr(bins[i].ac - wa);

			double n2 = bins[i].cnt;
			nerr *= (n1 * n2) / (n1 + n2);
			if (nerr >= err)
				continue;

			err = nerr;
			nn = i;
		}

		bin1.err = err;
		bin1.nn = nn;
	}

	static int PnnQuantize(const Color32Array &pixels, Color32Palette &palette, bool quan_sqrt)
	{
		DynamicArray<pnnbin> bins(65536);
		DynamicArray<int> heap(65537);
		double err, n1, n2;

		memset(bins, 0, bins.size() * sizeof(pnnbin));
		memset(heap, 0, heap.size() * sizeof(int));

		// Build histogram
		for (size_t i = 0; i < pixels.size(); i++)
		{
			// !!! Can throw gamma correction in here, but what to do about perceptual
			// !!! nonuniformity then?
			const Color32 &pixel = pixels[i];
			int index = getARGBIndex(pixel, hasSemiTransparency);
			auto& tb = bins[index];
			if (hasSemiTransparency)
				tb.ac += pixel.rgb.a;

			tb.rc += pixel.rgb.r;
			tb.gc += pixel.rgb.g;
			tb.bc += pixel.rgb.b;
			tb.cnt++;
		}

		// Cluster nonempty bins at one end of array
		int maxbins = 0;

		for (int i = 0; i < 65536; ++i)
		{
			if (!bins[i].cnt)
				continue;

			double d = 1.0 / (double)bins[i].cnt;
			if (hasSemiTransparency)
				bins[i].ac *= d;

			bins[i].rc *= d;
			bins[i].gc *= d;
			bins[i].bc *= d;

			if(quan_sqrt)
				bins[i].cnt = (int)_sqrt(bins[i].cnt);

			bins[maxbins] = bins[i];
			++maxbins;
		}

		for (int i = 0; i < maxbins - 1; ++i)
		{
			bins[i].fw = i + 1;
			bins[i + 1].bk = i;
		}

		int h, l, l2;

		// Initialize nearest neighbors and build heap of them
		for (int i = 0; i < maxbins; ++i)
		{
			findNearestNeighbor(bins, i);

			// Push slot on heap
			err = bins[i].err;
			for (l = ++heap[0]; l > 1; l = l2)
			{
				l2 = l >> 1;

				if (bins[h = heap[l2]].err <= err)
					break;

				heap[l] = h;
			}
			heap[l] = i;
		}

		// Merge bins which increase error the least
		int extbins = maxbins - static_cast<int>(palette.size());
		for (int i = 0; i < extbins; )
		{
			int b1;
			
			// Use heap to find which bins to merge
			for (;;)
			{
				pnnbin &tb = bins[b1 = heap[1]];	// One with least error 
													// Is stored error up to date?
				if ((tb.tm >= tb.mtm) && (bins[tb.nn].mtm <= tb.tm))
					break;

				if (tb.mtm == 0xFFFF) // Deleted node
					b1 = heap[1] = heap[heap[0]--];
				else // Too old error value
				{
					findNearestNeighbor(bins, b1);
					tb.tm = i;
				}

				// Push slot down
				err = bins[b1].err;
				for (l = 1; (l2 = l + l) <= heap[0]; l = l2)
				{
					if ((l2 < heap[0]) && (bins[heap[l2]].err > bins[heap[l2 + 1]].err))
						l2++;

					if (err <= bins[h = heap[l2]].err)
						break;

					heap[l] = h;
				}

				heap[l] = b1;
			}

			// Do a merge
			pnnbin &tb = bins[b1];
			pnnbin &nb = bins[tb.nn];
			n1 = tb.cnt;
			n2 = nb.cnt;
			double d = 1.0 / (n1 + n2);
			if (hasSemiTransparency)
				tb.ac = d * rint(n1 * tb.ac + n2 * nb.ac);

			tb.rc = d * rint(n1 * tb.rc + n2 * nb.rc);
			tb.gc = d * rint(n1 * tb.gc + n2 * nb.gc);
			tb.bc = d * rint(n1 * tb.bc + n2 * nb.bc);
			tb.cnt += nb.cnt;
			tb.mtm = ++i;

			// Unchain deleted bin
			bins[nb.bk].fw = nb.fw;
			bins[nb.fw].bk = nb.bk;
			nb.mtm = 0xFFFF;
		}

		// Fill palette
		uint32_t k = 0;
		for (int i = 0;; ++k)
		{
			uint8_t alpha = hasSemiTransparency ? (uint8_t)rint(bins[i].ac) : BYTE_MAX;
			palette[k] = MakeARGB(alpha, (uint8_t)rint(bins[i].rc), (uint8_t)rint(bins[i].gc), (uint8_t)rint(bins[i].bc));

			if (!(i = bins[i].fw))
				break;
		}

		return 0;
	}

	static uint32_t nearestColorIndex(const Color32Palette &palette, const Color32 &c)
	{
		uint32_t k = 0;

		uint32_t mindist = INT_MAX;
		for (uint32_t i = 0; i < palette.size(); ++i)
		{
			const Color32 &c2 = palette[i].m_color;
			uint32_t curdist;

			curdist = sqr(c2.rgb.a - c.rgb.a);
			if (curdist > mindist)
				continue;

			curdist += sqr(c2.rgb.r - c.rgb.r);
			if (curdist > mindist)
				continue;

			curdist += sqr(c2.rgb.g - c.rgb.g);
			if (curdist > mindist)
				continue;

			curdist += sqr(c2.rgb.b - c.rgb.b);
			if (curdist > mindist)
				continue;

			mindist = curdist;
			k = i;
		}

		return k;
	}

	static ColorMatch &bestMatchColorIndex(SortedArray<ColorMatch> &palette, const ColorMatch &color)
	{
		size_t index = static_cast<uint32_t>(palette.find(color));
		if (index == -1)
		{
			double bestAngle = DBL_MAX;
			double bestDistance = DBL_MAX;

			for (size_t pos = 0; pos < palette.size(); pos++)
			{
				ColorMatch &paletteColor = palette[pos];

				double y = (double)paletteColor.y - (double)color.y;
				double u = (double)paletteColor.u - (double)color.u;
				double v = (double)paletteColor.v - (double)color.v;

				double distance = sqrt(y*y + u*u + v*v);

				if (distance < bestDistance)
				{
					bestDistance = distance;
					index = pos;
				}
			}
		}

		return palette[index];
	}

	bool QuantizeImage(RGBAImage &source, Color32Palette &palette)
	{
		hasSemiTransparency = source.m_alpha;
		transparentColor = source.m_transparentColor;

		bool quan_sqrt = palette.size() > BYTE_MAX;
		PnnQuantize(source.m_pixels, palette, quan_sqrt);
		bool rc = DitherRGB(source, palette, nearestColorIndex, hasSemiTransparency);

		palette.sort();

		for (size_t i = 0; i < palette.size(); i++)
			palette[i].m_index = static_cast<uint32_t>(i);

		return rc;

	}

	bool RemapImage(RGBAImage &source, const Color32Palette &targetPalette)
	{
		if (targetPalette.size() < 2)
			return false;

		bool alpha = false;

		SortedArray<ColorMatch> palette;
		for (uint32_t i = 0; i < targetPalette.size(); i++)
		{
			ColorMatch cm(targetPalette[i].m_color, i);
			palette.add(cm);
		}

		Color32 *p = source.m_pixels;

		for (uint32_t y = 0; y < source.m_height; y++)
		{
			for (uint32_t x = 0; x < source.m_width; x++)
			{
				ColorMatch cm(*p, 0);
				ColorMatch &color = bestMatchColorIndex(palette, cm);
				p->value = color.m_color.value;

				p++;
			}
		}

		return true;
	}
}
