#include "utils/algorithms/bsearch.h"

bool binarySearch(const void *key, const void *base, size_t nelem, size_t width, void **pos, BSearchCompare *fcmp)
{
	char *kmin, *probe = (char *)base;
	char *eol = probe + nelem * width;
	size_t i;
	int j;

	kmin = (char *)base;

	while (nelem > 0)
	{
		i = nelem >> 1;
		probe = kmin + i * width;
		j = (*fcmp)(key, probe);

		if (j == 0)
		{
			if (pos)
				*pos = probe;

			return true;
		}
		else if (j < 0)
			nelem = i;
		else
		{
			kmin = probe + width;
			nelem = nelem - i - 1;
		}
	}

	// Make sure that we report the item where the key should be inserted (before)
	if (probe < eol)
	{
		if ((*fcmp)(key, probe) > 0)
			probe += width;
	}

	if (pos)
		*pos = probe;

	return false;
}
