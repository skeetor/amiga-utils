#ifndef _IMAGEFILE_H
#define _IMAGEFILE_H

#include "Color32Info.h"
#include "gfxImage.h"

typedef struct _ImageFile
{
	Color32 const &getTargetColor(Color32 const &color, Color32Palette const &targetColors) const;

	Color32Palette const &getTargetIndex(Color32 const &source, Color32Palette const &colors, Color32Palette const &targetColors, size_t &index) const;
} ImageFile;

#endif // _IMAGEFILE_H
