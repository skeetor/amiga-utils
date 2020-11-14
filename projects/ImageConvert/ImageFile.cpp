
#include "ImageFile.h"

Color32 const &ImageFile::getTargetColor(Color32 const &source, Color32Palette const &targetColors) const
{
	if (targetColors.empty())
		return source;

	size_t index = targetColors.nearestColorMatch(source, targetColors.size());

	return targetColors[index].m_color;
}

Color32Palette const &ImageFile::getTargetIndex(Color32 const &source, Color32Palette const &colors, Color32Palette const &targetColors, size_t &index) const
{
	Color32Info info;
	info = source;

	if (targetColors.empty())
	{
		index = colors.find(info);
		return colors;
	}

	index = targetColors.nearestColorMatch(source, targetColors.size());
	return targetColors;
}
