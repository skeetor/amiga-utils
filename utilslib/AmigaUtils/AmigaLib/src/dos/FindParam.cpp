#include <cstring>

int FindParam(int argc, char **argv, const char *name, int paramIndex, int start)
{
	char par = false;
	int index = 0;

	for (int i = start; i < argc; i++)
	{
		const char *p = argv[i];

		if (!par)
		{
			if (strcmp(p, name) == 0)
			{
				par = true;

				// Report the switch itself.
				if (!paramIndex)
					return i;
			}

			continue;
		}

		if (p[0] == '-')
			break;

		if (++index == paramIndex)
			return i;
	}

	if (!par)
		return -1;

	return 0;
}
