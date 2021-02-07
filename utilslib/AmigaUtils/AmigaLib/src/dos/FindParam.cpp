#include <cstring>

int FindParam(int argc, char **argv, const char *name, int paramIndex, int start)
{
	char par = false;
	uint32_t nameLen = strlen(name);
	int index = 0;

	for (int i = start; i < argc; i++)
	{
		const char *p = argv[i];

		if (!par)
		{
			if (strncmp(p, name, nameLen) == 0 && strlen(p) == nameLen)
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
