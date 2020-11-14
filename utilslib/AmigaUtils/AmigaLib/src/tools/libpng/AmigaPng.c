
#include <utils/tools/png/amiga_png.h>

int PngSetJmp(void *ptr)
{
	return setjmp(png_jmpbuf(ptr));
}
